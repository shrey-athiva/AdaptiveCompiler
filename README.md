# Adaptive AI-Assisted Compiler

Flex/Bison compiler for a small C-like language with context-aware error
recovery, semantic repair, and an LLM repair layer whose output is **verified
by the compiler itself**.

```
source ──► ircompiler (Flex/Bison) ──► diagnostics + REPAIR/DIAG markers + IR
              │                                   │
              │ rule-based repairs                │ if not confidently fixable
              │ (semicolon, ')', typo, declare)   ▼
              │                           LLM proposes ≤3 full-file fixes
              │                                   │
              └──────────── recompile each ◄──────┘
                            keep only fixes the compiler accepts
```

## Build and run

Needs `g++` (C++17), `flex`, `bison`, `make`, Python 3.9+.

```bash
# Ubuntu/Debian:  sudo apt install g++ flex bison make
# MSYS2 (UCRT64): pacman -S mingw-w64-ucrt-x86_64-gcc flex bison make
make                       # builds ./ircompiler  (ircompiler.exe on Windows)
pip install -r requirements.txt
python app.py              # http://127.0.0.1:5000
```

Enable AI repair with **one** of these keys, set before starting the app:

```bash
# Free option: Google Gemini (key: https://aistudio.google.com/apikey, no billing needed)
export GEMINI_API_KEY=...                    # PowerShell: $env:GEMINI_API_KEY="..."

# Or Claude (paid API; needs `pip install anthropic`)
export ANTHROPIC_API_KEY=sk-ant-...
```

If both are set, Claude is used; force one with `REPAIR_LLM_PROVIDER=gemini|anthropic`.
The Gemini provider uses only the Python standard library. Override models with
`GEMINI_MODEL` / `REPAIR_LLM_MODEL`; `python repair_cli.py --list-models` shows
which Gemini models your key can use. Free-tier rate limits are low (a few
requests per minute); each repair makes 1-2 requests.

CLI: `python repair_cli.py tests/cases/broken_while_header.txt` (`--force`, `--write`, `--json`).

Tests: `make test` (compiler regressions + LLM loop; no API key or network needed).

## How the LLM layer works (`llm_repair/`)

1. **Compile** the source. The compiler prints machine-readable markers
   (`RESULT`, `DIAG`, `REPAIR`, `CONF`, `SYMBOLS`) alongside the human report.
2. **Escalate or not** (`needs_llm`): the LLM is recommended only when the
   compiler could not fully recover (skipped statements, lexical errors) or an
   error has no deterministic repair at ≥85% confidence. Typos and missing
   `;` / `)` are fixed without an API call. The UI button is always available.
3. **Propose** (`llm_repair/providers.py`, Gemini or Claude): the model gets the language spec, numbered source, the
   compiler's report, its symbol table and its own suggestions, and returns up
   to 3 complete candidate programs as structured output (Claude: a
   `propose_repairs` tool call; Gemini: JSON-schema mode), with a JSON-in-text
   fallback if the model answers in prose.
4. **Verify**: every candidate is recompiled. Only `clean`/`warnings` results
   count as verified.
5. **Anti-cheating check**: when the original failed, its IR already omits the
   broken statements, so a real repair must make the IR *grow*. Candidates that
   merely delete the faulty code are flagged `removes_code` and ranked lower.
6. **Retry once** with the compiler's objections to rejected candidates fed
   back into the prompt, then rank: verified → not deleting code → fewest
   remaining errors → smallest edit → model confidence.

Privacy: the source is sent to the configured provider (Gemini or Claude) only when
AI repair runs. Free API tiers may allow the provider to use submitted content to
improve its products, so do not send confidential code; check the provider's terms.

## Compiler output markers

| Marker | Meaning |
|---|---|
| `RESULT:<r>` | `clean`, `warnings`, `recovered`, `semantic_error`, `syntax_error`, `lexical_error` |
| `DIAG:<sev>:<line>:<CODE>:<conf>` | one per diagnostic (`error`/`warning`/`recovery`) |
| `REPAIR:<KIND>:<args>` + `CONF:<c>` | deterministic repair the UI can apply |
| `SUMMARY:` / `SYMBOLS:` | counts; declared variables as `name:type:initialized` |

Exit code is 0 only for `clean`, `warnings` and `recovered`.

## Known limitations

- A skipped *declaration* loses its variable, so later uses report "not declared"
  (cascade). The LLM path handles these.
- Error recovery is statement-level, not block-aware: a broken `while (`/`if (`
  header can still end in "unrecovered syntax error".
- Confidence for syntax repairs are fixed rule priors (`recovery/recovery.h`), not learned.
- Nothing adapts yet: accepted/rejected repairs are not recorded.
