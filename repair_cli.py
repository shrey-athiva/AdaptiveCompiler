#!/usr/bin/env python3
"""Command-line front end for the compile -> LLM repair -> verify loop.

    python repair_cli.py program.txt              # show diagnostics and fixes
    python repair_cli.py program.txt --force      # call the LLM even if the
                                                  # compiler's own repairs suffice
    python repair_cli.py program.txt --write      # apply the best verified fix
                                                  # (original saved as .bak)
    python repair_cli.py program.txt --json       # machine-readable output

Exit status: 0 = program is clean or a verified fix exists, 1 = no verified
fix, 2 = usage/setup error (missing compiler or API key).
"""

import argparse
import json
import shutil
import sys
from pathlib import Path

from llm_repair import (CompilerNotFound, GeminiProvider, LLMRepairer,
                        LLMUnavailable, ProviderError, make_provider)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file", type=Path, nargs="?")
    ap.add_argument("--force", action="store_true",
                    help="call the LLM even when deterministic repairs are confident")
    ap.add_argument("--write", action="store_true",
                    help="overwrite the file with the best verified fix (keeps .bak)")
    ap.add_argument("--json", action="store_true", help="print JSON instead of text")
    ap.add_argument("--model", help="override the model id")
    ap.add_argument("--provider", choices=["gemini", "anthropic"],
                    help="LLM provider (default: whichever API key is set)")
    ap.add_argument("--list-models", action="store_true",
                    help="list Gemini models your key can use, then exit")
    ap.add_argument("--rounds", type=int, default=2, help="max LLM rounds (default 2)")
    args = ap.parse_args(argv)

    if args.list_models:
        try:
            for name in GeminiProvider().list_models():
                print(name)
        except (LLMUnavailable, ProviderError) as exc:
            print(f"error: {exc}", file=sys.stderr)
            return 2
        return 0

    if args.file is None:
        ap.error("the following arguments are required: file")

    try:
        source = args.file.read_text()
    except OSError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    try:
        provider = make_provider(name=args.provider, model=args.model)
        repairer = LLMRepairer(provider=provider, max_rounds=args.rounds)
        out = repairer.repair(source, force=args.force)
    except (LLMUnavailable, CompilerNotFound) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    if args.json:
        print(json.dumps({
            "used_llm": out.used_llm, "reason": out.reason, "error": out.error,
            "provider": out.provider, "model": out.model,
            "original_result": out.original.result, "attempts": out.attempts,
            "verified": out.best is not None,
            "candidates": [{
                "verified": c.verified, "removes_code": c.removes_code,
                "changed_lines": c.changed_lines, "explanation": c.explanation,
                "fixed_source": c.fixed_source} for c in out.candidates],
        }, indent=2))
    else:
        print(f"Original result: {out.original.result}")
        if out.original.report:
            print(out.original.report, "\n")
        print(f"LLM used: {out.used_llm} ({out.reason})")
        if out.provider:
            print(f"Provider: {out.provider} ({out.model})")
        if out.error:
            print(f"Error: {out.error}")
        for i, c in enumerate(out.candidates, 1):
            status = "VERIFIED" if c.verified else "REJECTED"
            flag = "  [may delete code - review]" if c.removes_code else ""
            print(f"\n--- candidate {i}: {status}, {c.changed_lines} line(s) changed{flag}")
            print(c.explanation)
            print(c.diff or "(no diff)")
            if not c.verified and c.remaining_issues:
                print("compiler said:\n" + c.remaining_issues)

    if out.original.ok:
        return 0

    if out.best is None:
        return 1

    if args.write:
        shutil.copyfile(args.file, args.file.with_suffix(args.file.suffix + ".bak"))
        args.file.write_text(out.best.fixed_source)
        print(f"\nApplied best verified fix to {args.file} (backup: {args.file}.bak)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
