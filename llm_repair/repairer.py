"""Propose -> verify -> rank LLM repair loop.

The LLM never gets the last word: every candidate it proposes is recompiled by
the real compiler and only candidates the compiler accepts are marked verified.
"""

from __future__ import annotations

import difflib
import json
import os
import re
from dataclasses import dataclass, field
from typing import Any, List, Optional, Tuple

from .compiler_runner import CompileResult, run_compiler
from .providers import (  # noqa: F401  (re-exported for backward compatibility)
    REPAIR_TOOL, TOOL_NAME, AnthropicProvider, GeminiProvider, LLMUnavailable,
    ProviderError, _extract_candidates, any_provider_configured, make_provider,
)

MAX_SOURCE_CHARS = 20_000
ESCALATION_THRESHOLD = 0.85   # deterministic repairs at/above this need no LLM

SYSTEM_PROMPT = """\
You repair programs written in a small C-like teaching language. A real \
compiler checks every program you return, so only repairs that compile \
cleanly are useful.

LANGUAGE (anything not listed is NOT supported - do not use it):
- Exactly one function, written `int main() { ... }` (return type may be int, \
float, char or bool). No parameters, no `return`, no function calls.
- Declarations: `int x = expr;` or `int x;`. Types: int, float, char, bool.
- Assignment: `x = expr;`
- Control flow: `if (a OP b) { ... }`, `if (...) { ... } else { ... }`, \
`while (a OP b) { ... }`. Braces are ALWAYS required. There is no `else if`. \
A condition is exactly one comparison; OP is one of < > <= >= == !=. \
No && || !.
- Expressions: + - * / and parentheses over int/float literals (5, 3.14), \
char literals ('a'), true/false, and variables.
- Types: float accepts int; int does NOT accept float; char accepts only \
char; bool accepts only bool. Arithmetic works only on int/float. char and \
bool may be compared only with == and !=.
- Variables must be declared before use. Statements end with ';'.
- No comments, strings, arrays, ++/--, for loops or compound assignment.

HOW TO REPAIR:
- Make the smallest edit that fixes the error and preserves the author's intent.
- Never delete statements or silently change program behaviour to make it \
compile. Keep every variable the author declared.
- Prefer fixing a typo over declaring a new variable, unless the name is \
clearly intended to be new.
- If several fixes are plausible, return up to 3 DISTINCT candidates, most \
likely first.
- Return each candidate as the complete program, same indentation style.
- Respond ONLY by calling the propose_repairs tool."""


# --------------------------------------------------------------------------
# Data model
# --------------------------------------------------------------------------
@dataclass
class RepairCandidate:
    fixed_source: str
    explanation: str
    llm_confidence: Optional[float]
    attempt: int
    verified: bool = False
    compile_result: Optional[CompileResult] = None
    diff: str = ""
    changed_lines: int = 0
    # True when the "fix" probably just deleted the faulty code instead of
    # repairing it (see _verify). Such candidates still verify, but rank lower
    # and the UI asks the user to review them.
    removes_code: bool = False
    remaining_issues: str = ""      # compiler report if not verified


@dataclass
class RepairOutcome:
    used_llm: bool
    reason: str
    original: CompileResult
    candidates: List[RepairCandidate] = field(default_factory=list)
    best: Optional[RepairCandidate] = None
    attempts: int = 0
    error: Optional[str] = None
    provider: Optional[str] = None
    model: Optional[str] = None


# --------------------------------------------------------------------------
# Escalation policy
# --------------------------------------------------------------------------
def needs_llm(result: CompileResult,
              threshold: float = ESCALATION_THRESHOLD) -> Tuple[bool, str]:
    """Decide whether the deterministic repairs are good enough.

    Escalate when the compiler could not fully recover, or when any error has
    no deterministic repair at or above `threshold` confidence.
    """
    if result.ok:
        return False, "program already compiles"

    if result.result in ("lexical_error", "syntax_error"):
        return True, f"compiler result '{result.result}': statements were skipped or unrecoverable"

    weak = [d for d in result.diagnostics
            if d.severity in ("error", "recovery") and d.confidence < threshold]

    if weak:
        d = weak[0]
        return True, (f"line {d.line}: {d.code} has no deterministic repair "
                      f"with confidence >= {threshold:.0%}")

    if not result.diagnostics:
        return True, "compiler reported a failure without structured diagnostics"

    return False, "all issues have high-confidence deterministic repairs"


# --------------------------------------------------------------------------
# Prompt construction
# --------------------------------------------------------------------------
def _numbered(source: str) -> str:
    return "\n".join(f"{i:>3} | {line}" for i, line in enumerate(source.splitlines(), 1))


def build_user_prompt(source: str, result: CompileResult,
                      failed: Optional[List[RepairCandidate]] = None) -> str:
    parts = ["Repair this program.\n", "SOURCE (line numbers added for reference):",
             _numbered(source), "", "COMPILER REPORT:", result.report or "(none)", ""]

    if result.symbols:
        syms = ", ".join(f"{s.name}: {s.type}" for s in result.symbols)
        parts += ["DECLARED VARIABLES (as the compiler saw them): " + syms, ""]

    if result.repairs:
        parts.append("DETERMINISTIC SUGGESTIONS ALREADY MADE BY THE COMPILER "
                     "(you may agree, improve on them, or reject them):")
        for r in result.repairs:
            parts.append(f"- {r.kind} {':'.join(r.args)} (confidence {r.confidence:.0%})")
        parts.append("")

    if failed:
        parts.append("YOUR PREVIOUS ATTEMPTS WERE REJECTED BY THE COMPILER. "
                     "Do not repeat them; fix the problems shown:")
        for i, c in enumerate(failed, 1):
            parts += [f"--- rejected attempt {i} ---", _numbered(c.fixed_source),
                      "compiler said:", c.remaining_issues or "(no details)", ""]

    parts.append("Call propose_repairs with your candidates.")
    return "\n".join(parts)


# --------------------------------------------------------------------------
# Response parsing
# --------------------------------------------------------------------------
_FENCE = re.compile(r"^```[a-zA-Z0-9_+-]*\n(.*?)\n?```\s*$", re.S)


def _clean_source(text: Any) -> Optional[str]:
    if not isinstance(text, str) or not text.strip():
        return None
    text = text.replace("\r\n", "\n")
    m = _FENCE.match(text.strip())
    if m:
        text = m.group(1)
    return text if text.endswith("\n") else text + "\n"


# --------------------------------------------------------------------------
# The repairer
# --------------------------------------------------------------------------
class LLMRepairer:
    def __init__(self, client: Any = None, model: Optional[str] = None,
                 provider: Any = None, max_rounds: int = 2,
                 max_candidates: int = 3):
        # Priority: explicit provider > an Anthropic-style client > chosen
        # lazily from the environment (so a missing key surfaces at repair time).
        if provider is None and client is not None:
            provider = AnthropicProvider(client=client, model=model)
        self._provider = provider
        self._model_override = model
        self.max_rounds = max_rounds
        self.max_candidates = max_candidates

    @staticmethod
    def is_configured() -> bool:
        return any_provider_configured()

    def _get_provider(self) -> Any:
        if self._provider is None:
            self._provider = make_provider(model=self._model_override)
        return self._provider

    def _ask(self, source: str, result: CompileResult,
             failed: Optional[List[RepairCandidate]]) -> List[dict]:
        return self._get_provider().propose(
            SYSTEM_PROMPT, build_user_prompt(source, result, failed))

    # -- verification --------------------------------------------------------
    def _verify(self, original_source: str, original: CompileResult,
                raw: dict, attempt: int) -> Optional[RepairCandidate]:
        fixed = _clean_source(raw.get("fixed_source"))
        if fixed is None:
            return None
        if fixed.strip() == original_source.strip():
            return None  # the model returned the input unchanged

        conf = raw.get("confidence")
        cand = RepairCandidate(
            fixed_source=fixed,
            explanation=str(raw.get("explanation", "")).strip(),
            llm_confidence=float(conf) if isinstance(conf, (int, float)) else None,
            attempt=attempt,
        )

        try:
            cand.compile_result = run_compiler(fixed)
        except Exception as exc:  # compiler crash / timeout counts as a failure
            cand.remaining_issues = f"compiler failed to run: {exc}"
            return cand

        cand.verified = cand.compile_result.ok
        if not cand.verified:
            cand.remaining_issues = cand.compile_result.report

        old_lines = original_source.splitlines()
        new_lines = fixed.splitlines()
        opcodes = [op for op in
                   difflib.SequenceMatcher(None, old_lines, new_lines).get_opcodes()
                   if op[0] != "equal"]

        cand.changed_lines = sum(max(i2 - i1, j2 - j1)
                                 for _, i1, i2, j1, j2 in opcodes)
        net_removed = sum(max(0, (i2 - i1) - (j2 - j1))
                          for _, i1, i2, j1, j2 in opcodes)

        # Compiler-grounded "did it just delete the broken code?" check.
        # When the original failed, the compiler's IR already OMITS the broken
        # statements, so a real repair must make the IR grow; a candidate that
        # deletes them leaves it unchanged. (A merely "recovered" original,
        # e.g. a missing ';', already has full IR, so equal length is fine.)
        grew = cand.compile_result.ir_length > original.ir_length
        same = cand.compile_result.ir_length == original.ir_length
        ir_ok = grew or (same and original.result == "recovered")
        cand.removes_code = (not ir_ok) or net_removed > 0

        cand.diff = "".join(difflib.unified_diff(
            [l + "\n" for l in old_lines], [l + "\n" for l in new_lines],
            fromfile="original", tofile="repaired", n=1))
        return cand

    @staticmethod
    def _rank_key(c: RepairCandidate):
        remaining = c.compile_result.errors if c.compile_result else 999
        # verified first, then no silent deletion, then fewest remaining
        # errors, then smallest edit, then the model's own confidence.
        return (not c.verified, c.removes_code, remaining, c.changed_lines,
                -(c.llm_confidence or 0.0))

    # -- main entry point -----------------------------------------------------
    def repair(self, source: str, force: bool = False) -> RepairOutcome:
        if len(source) > MAX_SOURCE_CHARS:
            raise ValueError(f"Source too large ({len(source)} > {MAX_SOURCE_CHARS} chars)")

        original = run_compiler(source)

        if original.ok:
            return RepairOutcome(False, "The program already compiles cleanly.", original)

        escalate, why = needs_llm(original)
        if not force and not escalate:
            return RepairOutcome(
                False,
                f"LLM not needed: {why}. Use the compiler's suggested repairs.",
                original)

        outcome = RepairOutcome(True, why if escalate else "LLM repair requested by user", original)
        verified_any = False
        failed: List[RepairCandidate] = []
        seen = set()

        for round_no in range(1, self.max_rounds + 1):
            outcome.attempts = round_no
            try:
                raw_candidates = self._ask(source, original, failed or None)
            except LLMUnavailable:
                raise
            except Exception as exc:
                outcome.error = f"LLM request failed: {type(exc).__name__}: {exc}"
                break

            round_new: List[RepairCandidate] = []
            for raw in raw_candidates[: self.max_candidates]:
                cand = self._verify(source, original, raw, round_no)
                if cand is None or cand.fixed_source in seen:
                    continue
                seen.add(cand.fixed_source)
                round_new.append(cand)

            outcome.candidates.extend(round_new)

            if any(c.verified for c in round_new):
                verified_any = True
                break

            # Nothing verified: feed the compiler's objections back once.
            failed = [c for c in round_new if not c.verified][:2]
            if not failed:
                break

        if self._provider is not None:
            outcome.provider = getattr(self._provider, "name", None)
            outcome.model = getattr(self._provider, "model", None)

        outcome.candidates.sort(key=self._rank_key)
        if verified_any and outcome.candidates and outcome.candidates[0].verified:
            outcome.best = outcome.candidates[0]
        elif not outcome.error and not outcome.candidates:
            outcome.error = "The model returned no usable candidates."

        return outcome
