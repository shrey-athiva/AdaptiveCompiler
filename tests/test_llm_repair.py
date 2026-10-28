"""Tests for the propose -> verify -> rank loop.

The LLM is replaced by a fake client that returns canned responses in the same
shape as the Anthropic SDK. Verification still runs the REAL compiler binary.
"""

import json
import os
import sys
import unittest
from types import SimpleNamespace
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from llm_repair import LLMRepairer, LLMUnavailable, needs_llm, run_compiler  # noqa: E402
from llm_repair.repairer import build_user_prompt  # noqa: E402


def tool_response(*candidates):
    block = SimpleNamespace(type="tool_use", name="propose_repairs",
                            input={"candidates": list(candidates)})
    return SimpleNamespace(content=[block])


def text_response(text):
    return SimpleNamespace(content=[SimpleNamespace(type="text", text=text)])


def cand(src, why="fix", conf=0.9):
    return {"fixed_source": src, "explanation": why, "confidence": conf}


class FakeClient:
    """Returns queued responses (or raises queued exceptions), records calls."""

    def __init__(self, *responses):
        self.responses = list(responses)
        self.calls = []
        self.messages = self

    def create(self, **kwargs):
        self.calls.append(kwargs)
        item = self.responses.pop(0)
        if isinstance(item, Exception):
            raise item
        return item


BROKEN_WHILE = "int main() {\nint x = 1;\nwhile (x > ) {\nx = 2;\n}\n}\n"
FIXED_WHILE = "int main() {\nint x = 1;\nwhile (x > 0) {\nx = 2;\n}\n}\n"
TYPE_ERR = "int main() {\nint x = 2.5;\n}\n"
TYPE_FIX_FLOAT = "int main() {\nfloat x = 2.5;\n}\n"


class Escalation(unittest.TestCase):
    def test_deterministic_cases_do_not_escalate(self):
        r = run_compiler("int main() {\nint count = 5;\ncuont = 7;\n}\n")
        self.assertFalse(needs_llm(r)[0])

    def test_unrecovered_and_low_confidence_escalate(self):
        self.assertTrue(needs_llm(run_compiler(BROKEN_WHILE))[0])
        self.assertTrue(needs_llm(run_compiler(TYPE_ERR))[0])

    def test_no_llm_call_when_not_needed_and_not_forced(self):
        client = FakeClient()
        out = LLMRepairer(client=client).repair(
            "int main() {\nint count = 5;\ncuont = 7;\n}\n")
        self.assertFalse(out.used_llm)
        self.assertEqual(client.calls, [])

    def test_force_calls_llm_even_when_deterministic_fix_exists(self):
        src = "int main() {\nint count = 5;\ncuont = 7;\n}\n"
        fixed = "int main() {\nint count = 5;\ncount = 7;\n}\n"
        client = FakeClient(tool_response(cand(fixed)))
        out = LLMRepairer(client=client).repair(src, force=True)
        self.assertTrue(out.used_llm)
        self.assertTrue(out.best.verified)

    def test_clean_program_short_circuits(self):
        client = FakeClient()
        out = LLMRepairer(client=client).repair(FIXED_WHILE, force=True)
        self.assertFalse(out.used_llm)
        self.assertEqual(client.calls, [])


class VerifyAndRank(unittest.TestCase):
    def test_compiler_verifies_candidates(self):
        client = FakeClient(tool_response(
            cand("int main() {\nint x = 1;\nwhile (x > ) {\nx = 9;\n}\n}\n", conf=0.99),  # still broken
            cand(FIXED_WHILE, conf=0.6)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(len(out.candidates), 2)
        # The verified one wins even though the model was less confident in it.
        self.assertTrue(out.best.verified)
        self.assertEqual(out.best.fixed_source, FIXED_WHILE)
        self.assertFalse(out.candidates[1].verified)
        self.assertIn("Syntax Error", out.candidates[1].remaining_issues)

    def test_deleting_the_broken_code_is_flagged_and_ranked_lower(self):
        deleted = "int main() {\nint x = 1;\n}\n"      # compiles, but dropped the loop
        client = FakeClient(tool_response(cand(deleted, conf=0.99), cand(FIXED_WHILE)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        by_src = {c.fixed_source: c for c in out.candidates}
        self.assertTrue(by_src[deleted].verified)
        self.assertTrue(by_src[deleted].removes_code)
        self.assertFalse(by_src[FIXED_WHILE].removes_code)
        self.assertEqual(out.best.fixed_source, FIXED_WHILE)

    def test_diff_and_change_count(self):
        client = FakeClient(tool_response(cand(FIXED_WHILE)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(out.best.changed_lines, 1)
        self.assertIn("-while (x > ) {", out.best.diff)
        self.assertIn("+while (x > 0) {", out.best.diff)

    def test_unchanged_and_duplicate_candidates_are_dropped(self):
        client = FakeClient(tool_response(cand(BROKEN_WHILE), cand(FIXED_WHILE),
                                          cand(FIXED_WHILE)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(len(out.candidates), 1)

    def test_code_fences_are_stripped(self):
        client = FakeClient(tool_response(cand("```c\n" + FIXED_WHILE + "```")))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertTrue(out.best.verified)


class RetryWithFeedback(unittest.TestCase):
    def test_second_round_receives_compiler_objections(self):
        still_broken = "int main() {\nint x = 1;\nwhile (x > ) {\nx = 3;\n}\n}\n"
        client = FakeClient(tool_response(cand(still_broken)),
                            tool_response(cand(FIXED_WHILE)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(len(client.calls), 2)
        self.assertEqual(out.attempts, 2)
        second_prompt = client.calls[1]["messages"][0]["content"]
        self.assertIn("REJECTED BY THE COMPILER", second_prompt)
        self.assertIn("Syntax Error", second_prompt)
        self.assertTrue(out.best.verified)
        self.assertEqual(out.best.attempt, 2)

    def test_stops_after_max_rounds_without_verified_fix(self):
        def broken(n):
            return f"int main() {{\nint x = {n};\nwhile (x > ) {{\nx = 2;\n}}\n}}\n"
        client = FakeClient(tool_response(cand(broken(5))), tool_response(cand(broken(6))))
        out = LLMRepairer(client=client, max_rounds=2).repair(BROKEN_WHILE)
        self.assertEqual(len(client.calls), 2)
        self.assertIsNone(out.best)
        self.assertEqual(len(out.candidates), 2)

    def test_does_not_retry_once_a_candidate_verifies(self):
        client = FakeClient(tool_response(cand(FIXED_WHILE)))
        LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(len(client.calls), 1)


class RobustParsing(unittest.TestCase):
    def test_json_in_text_fallback(self):
        import json
        payload = json.dumps({"candidates": [cand(FIXED_WHILE)]})
        client = FakeClient(text_response("Here you go:\n" + payload))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertTrue(out.best.verified)

    def test_garbage_response_gives_clear_error(self):
        client = FakeClient(text_response("I cannot help with that."))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertIsNone(out.best)
        self.assertIn("no usable candidates", out.error)

    def test_malformed_candidates_are_skipped(self):
        client = FakeClient(tool_response({"explanation": "no source"},
                                          {"fixed_source": 42}, cand(FIXED_WHILE)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertEqual(len(out.candidates), 1)
        self.assertTrue(out.best.verified)

    def test_api_failure_is_reported_not_raised(self):
        client = FakeClient(RuntimeError("boom"))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertIn("boom", out.error)
        self.assertIsNone(out.best)

    def test_missing_api_key_raises_llm_unavailable(self):
        names = ("ANTHROPIC_API_KEY", "GEMINI_API_KEY", "GOOGLE_API_KEY",
                 "REPAIR_LLM_PROVIDER")
        saved = {n: os.environ.pop(n) for n in names if n in os.environ}
        try:
            with self.assertRaises(LLMUnavailable):
                LLMRepairer().repair(BROKEN_WHILE)
        finally:
            os.environ.update(saved)

    def test_oversized_source_rejected(self):
        with self.assertRaises(ValueError):
            LLMRepairer(client=FakeClient()).repair("x" * 25_000)


class RequestShape(unittest.TestCase):
    def test_request_uses_auto_tool_choice_and_tool(self):
        client = FakeClient(tool_response(cand(TYPE_FIX_FLOAT)))
        LLMRepairer(client=client, model="m-test").repair(TYPE_ERR)
        call = client.calls[0]
        self.assertEqual(call["model"], "m-test")
        self.assertEqual(call["tool_choice"], {"type": "auto"})
        self.assertEqual(call["tools"][0]["name"], "propose_repairs")

    def test_prompt_contains_diagnostics_symbols_and_suggestions(self):
        src = "int main() {\nint count = 5;\nzzz = 7;\n}\n"
        prompt = build_user_prompt(src, run_compiler(src))
        self.assertIn("  3 | zzz = 7;", prompt)               # numbered source
        self.assertIn("count: int", prompt)                    # symbol table
        self.assertIn("DECLARE_VARIABLE", prompt)              # compiler's own suggestion
        self.assertIn("not declared", prompt)                  # human report



try:
    import anthropic
    try:                      # anthropic SDK 2.x ships its own "httpx2"
        import httpx2 as httpx
    except ImportError:       # older SDKs use plain httpx
        import httpx
except ImportError:  # pragma: no cover
    anthropic = None


@unittest.skipIf(anthropic is None, "anthropic SDK not installed")
class RealSdkObjects(unittest.TestCase):
    """Run the loop through the REAL anthropic SDK with a mocked HTTP layer, so
    response parsing is exercised against the SDK's actual types."""

    def _client(self, body, seen):
        def handler(request):
            seen.append(json.loads(request.content))
            return httpx.Response(200, json=body)

        return anthropic.Anthropic(
            api_key="test-key", max_retries=0,
            http_client=httpx.Client(transport=httpx.MockTransport(handler)))

    def _message(self, content):
        return {"id": "msg_test", "type": "message", "role": "assistant",
                "model": "test-model", "content": content,
                "stop_reason": "tool_use", "stop_sequence": None,
                "usage": {"input_tokens": 1, "output_tokens": 1}}

    def test_tool_use_block_round_trip(self):
        seen = []
        body = self._message([
            {"type": "text", "text": "Fixing the loop."},
            {"type": "tool_use", "id": "toolu_1", "name": "propose_repairs",
             "input": {"candidates": [cand(FIXED_WHILE, "added operand")]}}])
        out = LLMRepairer(client=self._client(body, seen),
                          model="test-model").repair(BROKEN_WHILE)
        self.assertTrue(out.best.verified)
        self.assertEqual(out.best.explanation, "added operand")
        # What we actually sent over the wire:
        self.assertEqual(seen[0]["model"], "test-model")
        self.assertEqual(seen[0]["tool_choice"], {"type": "auto"})
        self.assertEqual(seen[0]["tools"][0]["name"], "propose_repairs")
        self.assertIn("compiler", seen[0]["system"])

    def test_text_only_json_response_via_sdk(self):
        body = self._message([{"type": "text",
                               "text": json.dumps({"candidates": [cand(FIXED_WHILE)]})}])
        out = LLMRepairer(client=self._client(body, [])).repair(BROKEN_WHILE)
        self.assertTrue(out.best.verified)

    def test_http_error_becomes_outcome_error(self):
        def handler(request):
            return httpx.Response(401, json={"type": "error", "error": {
                "type": "authentication_error", "message": "invalid x-api-key"}})
        client = anthropic.Anthropic(
            api_key="bad", max_retries=0,
            http_client=httpx.Client(transport=httpx.MockTransport(handler)))
        out = LLMRepairer(client=client).repair(BROKEN_WHILE)
        self.assertIsNone(out.best)
        self.assertIn("AuthenticationError", out.error)


if __name__ == "__main__":
    unittest.main()
