"""Adaptive Compiler Studio - Flask front end.

    python app.py                       # http://127.0.0.1:5000

Enable the "AI repair" button with ONE of these keys (free Gemini key:
https://aistudio.google.com/apikey):

Environment variables:
    GEMINI_API_KEY       Google Gemini (free tier available)  [or GOOGLE_API_KEY]
    ANTHROPIC_API_KEY    Claude (paid API)
    REPAIR_LLM_PROVIDER  gemini | anthropic  (default: whichever key is set)
    GEMINI_MODEL         Gemini model id (default: tries several, see providers.py)
    REPAIR_LLM_MODEL     Claude model id
    IRCOMPILER           path to the compiler binary (default: ./ircompiler[.exe])
    MSYS2_BIN            Windows only: MSYS2 runtime dir (default C:\\msys64\\ucrt64\\bin)
    REPAIRS_PER_MINUTE   rate limit for /repair (default 10)
    FLASK_DEBUG=1        enable Flask debug mode (off by default)
"""

import logging
import os
import subprocess
import time
from collections import deque

from flask import Flask, jsonify, render_template, request

from llm_repair import (
    CompilerNotFound,
    LLMRepairer,
    LLMUnavailable,
    needs_llm,
    run_compiler,
)
from llm_repair.repairer import MAX_SOURCE_CHARS

app = Flask(__name__)
log = logging.getLogger("adaptive-compiler")
logging.basicConfig(level=logging.INFO, format="%(levelname)s %(message)s")

repairer = LLMRepairer()

# Tiny in-memory rate limiter so a stuck button can't run up API costs.
_RATE_LIMIT = int(os.environ.get("REPAIRS_PER_MINUTE", "10"))
_recent_repairs = deque()


def _rate_limited() -> bool:
    now = time.monotonic()
    while _recent_repairs and now - _recent_repairs[0] > 60:
        _recent_repairs.popleft()
    if len(_recent_repairs) >= _RATE_LIMIT:
        return True
    _recent_repairs.append(now)
    return False


def _get_source():
    """Return (source, error_response)."""
    data = request.get_json(silent=True) or {}
    source = data.get("code", "")

    if not isinstance(source, str) or not source.strip():
        return None, (jsonify(success=False, output="Please enter some source code."), 400)
    if len(source) > MAX_SOURCE_CHARS:
        return None, (jsonify(success=False,
                              output=f"Source too large (max {MAX_SOURCE_CHARS} characters)."), 413)
    return source, None


def _compile_dict(res):
    escalate, reason = needs_llm(res)
    return {
        "success": res.returncode == 0,
        "output": res.raw_output or (
            f"The compiler returned no output.\nExit code: {res.returncode}"),
        "result": res.result,
        "ok": res.ok,
        "errors": res.errors,
        "warnings": res.warnings,
        "recoveries": res.recoveries,
        "symbols": [vars(s) for s in res.symbols],
        "diagnostics": [vars(d) for d in res.diagnostics],
        "repairs": [vars(r) for r in res.repairs],
        "needs_llm": escalate,
        "needs_llm_reason": reason,
        "llm_configured": LLMRepairer.is_configured(),
    }


def _outcome_dict(out):
    return {
        "used_llm": out.used_llm,
        "reason": out.reason,
        "error": out.error,
        "attempts": out.attempts,
        "provider": out.provider,
        "model": out.model,
        "original_result": out.original.result,
        "verified": out.best is not None,
        "best_index": 0 if out.best else None,
        "candidates": [{
            "fixed_source": c.fixed_source,
            "explanation": c.explanation,
            "llm_confidence": c.llm_confidence,
            "attempt": c.attempt,
            "verified": c.verified,
            "removes_code": c.removes_code,
            "changed_lines": c.changed_lines,
            "diff": c.diff,
            "remaining_issues": c.remaining_issues,
            "result": c.compile_result.result if c.compile_result else "unknown",
        } for c in out.candidates],
    }


@app.route("/")
def home():
    return render_template("index.html")


@app.route("/compile", methods=["POST"])
def compile_code():
    source, err = _get_source()
    if err:
        return err

    try:
        return jsonify(_compile_dict(run_compiler(source)))
    except CompilerNotFound as exc:
        return jsonify(success=False, output=str(exc)), 500
    except subprocess.TimeoutExpired:
        return jsonify(success=False, output="Compiler timed out after 10 seconds."), 504
    except OSError as exc:
        return jsonify(success=False, output=f"Could not launch compiler: {exc}"), 500
    except Exception as exc:  # noqa: BLE001
        log.exception("unexpected compile failure")
        return jsonify(success=False, output=f"Unexpected compiler error: {exc}"), 500


@app.route("/repair", methods=["POST"])
def repair_code():
    """Ask the LLM for fixes; every candidate is verified by the compiler."""
    source, err = _get_source()
    if err:
        return err

    if _rate_limited():
        return jsonify(success=False,
                       output="Too many repair requests. Please wait a minute."), 429

    force = bool((request.get_json(silent=True) or {}).get("force", True))

    try:
        outcome = repairer.repair(source, force=force)
        log.info("repair: used_llm=%s verified=%s attempts=%s candidates=%d",
                 outcome.used_llm, outcome.best is not None, outcome.attempts,
                 len(outcome.candidates))
        return jsonify(success=True, **_outcome_dict(outcome))
    except LLMUnavailable as exc:
        return jsonify(success=False, output=str(exc)), 503
    except CompilerNotFound as exc:
        return jsonify(success=False, output=str(exc)), 500
    except subprocess.TimeoutExpired:
        return jsonify(success=False, output="Compiler timed out after 10 seconds."), 504
    except Exception as exc:  # noqa: BLE001
        log.exception("unexpected repair failure")
        return jsonify(success=False, output=f"Unexpected repair error: {exc}"), 500


if __name__ == "__main__":
    app.run(host="127.0.0.1", debug=os.environ.get("FLASK_DEBUG") == "1")
