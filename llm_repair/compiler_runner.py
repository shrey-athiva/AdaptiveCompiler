"""Run the compiler binary and parse its machine-readable markers.

The compiler prints human-readable diagnostics plus marker lines:

    REPAIR:<KIND>:<args>          deterministic repair suggestion
    CONF:<0..1>                   confidence of the REPAIR just above
    DIAG:<severity>:<line>:<CODE>:<confidence>
    RESULT:<clean|warnings|recovered|semantic_error|syntax_error|lexical_error>
    SUMMARY:errors=N warnings=N recoveries=N
    SYMBOLS:<name>:<type>:<initialized>,...
"""

from __future__ import annotations

import os
import re
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

BASE_DIR = Path(__file__).resolve().parent.parent
IR_MARKER = "===== Intermediate Representation ====="
MARKER_PREFIXES = ("REPAIR:", "CONF:", "DIAG:", "RESULT:", "SUMMARY:", "SYMBOLS:")

# Results that mean "this program compiles without needing any repair".
OK_RESULTS = ("clean", "warnings")


class CompilerNotFound(RuntimeError):
    pass


@dataclass
class Diagnostic:
    severity: str          # error | warning | recovery
    line: int
    code: str
    confidence: float


@dataclass
class Repair:
    kind: str              # ADD_SEMICOLON, ADD_RPAREN, REPLACE_IDENTIFIER, DECLARE_VARIABLE
    args: List[str]
    confidence: float = 0.0


@dataclass
class Symbol:
    name: str
    type: str
    initialized: bool


@dataclass
class CompileResult:
    returncode: int
    raw_output: str
    result: str = "unknown"
    errors: int = 0
    warnings: int = 0
    recoveries: int = 0
    symbols: List[Symbol] = field(default_factory=list)
    diagnostics: List[Diagnostic] = field(default_factory=list)
    repairs: List[Repair] = field(default_factory=list)
    report: str = ""       # human-readable diagnostics, markers stripped
    ir: str = ""

    @property
    def ok(self) -> bool:
        return self.result in OK_RESULTS

    @property
    def ir_length(self) -> int:
        return len([l for l in self.ir.splitlines() if l.strip()])


def find_compiler() -> Path:
    override = os.environ.get("IRCOMPILER")
    if override:
        path = Path(override)
    else:
        name = "ircompiler.exe" if os.name == "nt" else "ircompiler"
        path = BASE_DIR / name

    if not path.is_file():
        raise CompilerNotFound(
            f"Compiler not found: {path}. Build it with `make` "
            "(or set the IRCOMPILER environment variable)."
        )
    return path


def _runtime_env() -> dict:
    """On Windows/MSYS2 the binary may need the MSYS2 runtime DLLs on PATH."""
    env = os.environ.copy()
    if os.name == "nt":
        msys = os.environ.get("MSYS2_BIN", r"C:\msys64\ucrt64\bin")
        if os.path.isdir(msys):
            env["PATH"] = msys + os.pathsep + env.get("PATH", "")
    return env


def parse_output(output: str, returncode: int = 0) -> CompileResult:
    res = CompileResult(returncode=returncode, raw_output=output)

    head, _, tail = output.partition(IR_MARKER)
    res.ir = tail.replace("=======================================", "").strip()

    report_lines: List[str] = []
    last_repair: Optional[Repair] = None

    for line in head.splitlines():
        text = line.strip()

        if text.startswith("RESULT:"):
            res.result = text[len("RESULT:"):].strip()
        elif text.startswith("SUMMARY:"):
            for key, val in re.findall(r"(\w+)=(\d+)", text):
                if key == "errors":
                    res.errors = int(val)
                elif key == "warnings":
                    res.warnings = int(val)
                elif key == "recoveries":
                    res.recoveries = int(val)
        elif text.startswith("SYMBOLS:"):
            body = text[len("SYMBOLS:"):]
            for item in filter(None, body.split(",")):
                parts = item.split(":")
                if len(parts) == 3:
                    res.symbols.append(Symbol(parts[0], parts[1], parts[2] == "1"))
        elif text.startswith("DIAG:"):
            parts = text.split(":")
            if len(parts) == 5:
                try:
                    res.diagnostics.append(
                        Diagnostic(parts[1], int(parts[2]), parts[3], float(parts[4]))
                    )
                except ValueError:
                    pass
        elif text.startswith("REPAIR:"):
            parts = text.split(":")
            last_repair = Repair(kind=parts[1], args=parts[2:])
            res.repairs.append(last_repair)
        elif text.startswith("CONF:"):
            if last_repair is not None:
                try:
                    last_repair.confidence = float(text[len("CONF:"):])
                except ValueError:
                    pass
        elif text and text != "Enter source code:":
            report_lines.append(line.rstrip())

    res.report = re.sub(r"\n{3,}", "\n\n", "\n".join(report_lines)).strip()
    return res


def run_compiler(source: str, timeout: float = 10.0) -> CompileResult:
    compiler = find_compiler()

    proc = subprocess.run(
        [str(compiler)],
        input=source if source.endswith("\n") else source + "\n",
        text=True,
        capture_output=True,
        timeout=timeout,
        cwd=str(BASE_DIR),
        env=_runtime_env(),
    )

    return parse_output(proc.stdout + proc.stderr, proc.returncode)
