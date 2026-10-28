"""Gemini provider tests.

The HTTP layer is faked (responses follow Google's documented generateContent
format); verification of every candidate still runs the REAL compiler binary.
"""

import json
import os
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from llm_repair import (GeminiProvider, LLMRepairer, LLMUnavailable,  # noqa: E402
                        ProviderError, make_provider)
from llm_repair.providers import AnthropicProvider  # noqa: E402

BROKEN = "int main() {\nint x = 1;\nwhile (x > ) {\nx = 2;\n}\n}\n"
FIXED = "int main() {\nint x = 1;\nwhile (x > 0) {\nx = 2;\n}\n}\n"

ENV_NAMES = ("ANTHROPIC_API_KEY", "GEMINI_API_KEY", "GOOGLE_API_KEY",
             "REPAIR_LLM_PROVIDER", "GEMINI_MODEL")


def ok(payload_text):
    """A 200 generateContent response carrying `payload_text`."""
    return 200, {"candidates": [{"content": {"role": "model",
                 "parts": [{"text": payload_text}]}, "finishReason": "STOP"}]}


def cands(*sources):
    return json.dumps({"candidates": [
        {"fixed_source": s, "explanation": "fix", "confidence": 0.8} for s in sources]})


def err(status, message):
    return status, {"error": {"code": status, "message": message, "status": "X"}}


class FakeHttp:
    def __init__(self, *responses):
        self.responses = list(responses)
        self.calls = []

    def __call__(self, method, url, headers, body, timeout):
        self.calls.append({"method": method, "url": url, "headers": headers, "body": body})
        item = self.responses.pop(0)
        if isinstance(item, Exception):
            raise item
        return item


class CleanEnv(unittest.TestCase):
    def setUp(self):
        self._saved = {n: os.environ.pop(n) for n in ENV_NAMES if n in os.environ}

    def tearDown(self):
        for n in ENV_NAMES:
            os.environ.pop(n, None)
        os.environ.update(self._saved)


class RequestFormat(CleanEnv):
    def test_request_matches_documented_rest_shape(self):
        http = FakeHttp(ok(cands(FIXED)))
        p = GeminiProvider(api_key="SECRET-KEY-123", model="gemini-test", http=http)
        p.propose("SYSTEM TEXT", "USER TEXT")
        call = http.calls[0]
        self.assertEqual(call["method"], "POST")
        self.assertTrue(call["url"].endswith("/models/gemini-test:generateContent"))
        self.assertEqual(call["headers"], {"x-goog-api-key": "SECRET-KEY-123"})
        body = call["body"]
        self.assertEqual(body["systemInstruction"]["parts"][0]["text"], "SYSTEM TEXT")
        self.assertEqual(body["contents"][0]["parts"][0]["text"], "USER TEXT")
        cfg = body["generationConfig"]
        self.assertEqual(cfg["responseMimeType"], "application/json")
        self.assertIn("candidates", cfg["responseJsonSchema"]["properties"])

    def test_key_is_never_placed_in_the_url(self):
        http = FakeHttp(ok(cands(FIXED)))
        GeminiProvider(api_key="SECRET-KEY-123", http=http).propose("s", "u")
        self.assertNotIn("SECRET-KEY-123", http.calls[0]["url"])

    def test_key_read_from_either_env_variable(self):
        os.environ["GOOGLE_API_KEY"] = "k-google"
        http = FakeHttp(ok(cands(FIXED)))
        GeminiProvider(http=http).propose("s", "u")
        self.assertEqual(http.calls[0]["headers"]["x-goog-api-key"], "k-google")
        os.environ["GEMINI_API_KEY"] = "k-gemini"       # GEMINI_API_KEY takes priority
        http = FakeHttp(ok(cands(FIXED)))
        GeminiProvider(http=http).propose("s", "u")
        self.assertEqual(http.calls[0]["headers"]["x-goog-api-key"], "k-gemini")

    def test_missing_key_raises_unavailable_with_signup_hint(self):
        with self.assertRaises(LLMUnavailable) as ctx:
            GeminiProvider(http=FakeHttp()).propose("s", "u")
        self.assertIn("aistudio.google.com/apikey", str(ctx.exception))


class ResponseParsing(CleanEnv):
    def _propose(self, response):
        return GeminiProvider(api_key="k", http=FakeHttp(response)).propose("s", "u")

    def test_parses_json_candidates(self):
        out = self._propose(ok(cands(FIXED, BROKEN)))
        self.assertEqual([c["fixed_source"] for c in out], [FIXED, BROKEN])

    def test_parses_json_wrapped_in_prose_or_fences(self):
        out = self._propose(ok("Here you go:\n```json\n" + cands(FIXED) + "\n```"))
        self.assertEqual(len(out), 1)

    def test_joins_split_text_parts_and_skips_thought_parts(self):
        text = cands(FIXED)
        half = len(text) // 2
        resp = 200, {"candidates": [{"content": {"parts": [
            {"text": "internal reasoning", "thought": True},
            {"text": text[:half]}, {"text": text[half:]}]}}]}
        self.assertEqual(len(self._propose(resp)), 1)

    def test_empty_or_garbage_response_gives_no_candidates(self):
        self.assertEqual(self._propose((200, {})), [])
        self.assertEqual(self._propose(ok("I cannot help.")), [])

    def test_blocked_prompt_is_a_clear_error(self):
        with self.assertRaises(ProviderError) as ctx:
            self._propose((200, {"promptFeedback": {"blockReason": "SAFETY"}}))
        self.assertIn("SAFETY", str(ctx.exception))


class ErrorHandling(CleanEnv):
    def _fail(self, *responses):
        p = GeminiProvider(api_key="k", http=FakeHttp(*responses), sleep=lambda s: None)
        with self.assertRaises(ProviderError) as ctx:
            p.propose("s", "u")
        return str(ctx.exception)

    def test_invalid_key_message(self):
        msg = self._fail(err(400, "API key not valid. Please pass a valid API key."))
        self.assertIn("rejected the API key", msg)

    def test_forbidden_key_message(self):
        self.assertIn("rejected the API key", self._fail(err(403, "forbidden")))

    def test_rate_limit_after_one_retry_has_actionable_message(self):
        http = FakeHttp(err(429, "quota"), err(429, "quota"))
        p = GeminiProvider(api_key="k", model="m", http=http, sleep=lambda s: None)
        with self.assertRaises(ProviderError) as ctx:
            p.propose("s", "u")
        self.assertEqual(len(http.calls), 2)
        self.assertIn("Wait a minute", str(ctx.exception))

    def test_transient_429_succeeds_on_retry(self):
        slept = []
        http = FakeHttp(err(429, "slow down"), ok(cands(FIXED)))
        p = GeminiProvider(api_key="k", model="m", http=http, sleep=slept.append)
        self.assertEqual(len(p.propose("s", "u")), 1)
        self.assertEqual(len(slept), 1)

    def test_overloaded_503_is_retried_then_reported(self):
        self.assertIn("overloaded", self._fail(err(503, "busy"), err(503, "busy")))

    def test_network_failure_surfaces_as_provider_error(self):
        msg = self._fail(ProviderError("Could not reach the Gemini API: offline"))
        self.assertIn("offline", msg)


class ModelFallback(CleanEnv):
    def test_unavailable_models_are_skipped_in_order(self):
        http = FakeHttp(err(404, "not found"), err(404, "not found"), ok(cands(FIXED)))
        p = GeminiProvider(api_key="k", http=http)
        self.assertEqual(len(p.propose("s", "u")), 1)
        urls = [c["url"] for c in http.calls]
        self.assertEqual(len(urls), 3)
        self.assertTrue(urls[2].endswith(p.model + ":generateContent"))   # remembers the winner
        self.assertNotEqual(urls[0], urls[2])

    def test_all_models_unavailable_points_to_list_models(self):
        http = FakeHttp(*[err(404, "nf")] * 3)
        with self.assertRaises(ProviderError) as ctx:
            GeminiProvider(api_key="k", http=http).propose("s", "u")
        self.assertIn("--list-models", str(ctx.exception))

    def test_explicit_model_is_used_alone(self):
        http = FakeHttp(err(404, "nf"))
        with self.assertRaises(ProviderError):
            GeminiProvider(api_key="k", model="only-this", http=http).propose("s", "u")
        self.assertEqual(len(http.calls), 1)

    def test_gemini_model_env_var(self):
        os.environ["GEMINI_MODEL"] = "from-env"
        self.assertEqual(GeminiProvider(api_key="k").models, ["from-env"])

    def test_list_models_filters_to_generate_content(self):
        http = FakeHttp((200, {"models": [
            {"name": "models/a-flash", "supportedGenerationMethods": ["generateContent"]},
            {"name": "models/embed", "supportedGenerationMethods": ["embedContent"]},
            {"name": "models/b-flash", "supportedGenerationMethods": ["generateContent", "x"]}]}))
        self.assertEqual(GeminiProvider(api_key="k", http=http).list_models(),
                         ["a-flash", "b-flash"])
        self.assertEqual(http.calls[0]["method"], "GET")


class ProviderSelection(CleanEnv):
    def test_auto_picks_gemini_when_only_gemini_key_set(self):
        os.environ["GEMINI_API_KEY"] = "k"
        self.assertEqual(make_provider().name, "gemini")

    def test_auto_prefers_anthropic_when_both_set(self):
        os.environ["GEMINI_API_KEY"] = "k"
        os.environ["ANTHROPIC_API_KEY"] = "k"
        self.assertIsInstance(make_provider(), AnthropicProvider)

    def test_explicit_provider_overrides_auto(self):
        os.environ["GEMINI_API_KEY"] = "k"
        os.environ["ANTHROPIC_API_KEY"] = "k"
        os.environ["REPAIR_LLM_PROVIDER"] = "gemini"
        self.assertEqual(make_provider().name, "gemini")

    def test_unknown_provider_and_no_keys_are_clear_errors(self):
        os.environ["REPAIR_LLM_PROVIDER"] = "bard"
        with self.assertRaises(LLMUnavailable) as ctx:
            make_provider()
        self.assertIn("gemini", str(ctx.exception))
        del os.environ["REPAIR_LLM_PROVIDER"]
        with self.assertRaises(LLMUnavailable) as ctx:
            make_provider()
        self.assertIn("GEMINI_API_KEY", str(ctx.exception))

    def test_is_configured_true_for_either_key(self):
        self.assertFalse(LLMRepairer.is_configured())
        os.environ["GEMINI_API_KEY"] = "k"
        self.assertTrue(LLMRepairer.is_configured())


class EndToEndWithRealCompiler(CleanEnv):
    def test_gemini_candidates_are_verified_by_the_compiler(self):
        deleted = "int main() {\nint x = 1;\n}\n"
        http = FakeHttp(ok(cands(deleted, BROKEN.replace("x = 2", "x = 9"), FIXED)))
        repairer = LLMRepairer(provider=GeminiProvider(api_key="k", model="m", http=http))
        out = repairer.repair(BROKEN)
        self.assertEqual(out.provider, "gemini")
        self.assertEqual(out.model, "m")
        self.assertEqual(out.best.fixed_source, FIXED)      # real fix outranks deletion
        by_src = {c.fixed_source: c for c in out.candidates}
        self.assertTrue(by_src[deleted].removes_code)
        self.assertEqual([c.verified for c in out.candidates], [True, True, False])

    def test_retry_round_feeds_compiler_errors_back_to_gemini(self):
        still_broken = BROKEN.replace("x = 2", "x = 3")
        http = FakeHttp(ok(cands(still_broken)), ok(cands(FIXED)))
        out = LLMRepairer(provider=GeminiProvider(api_key="k", model="m", http=http)).repair(BROKEN)
        self.assertEqual(len(http.calls), 2)
        second_prompt = http.calls[1]["body"]["contents"][0]["parts"][0]["text"]
        self.assertIn("REJECTED BY THE COMPILER", second_prompt)
        self.assertTrue(out.best.verified)

    def test_provider_error_becomes_outcome_error_not_exception(self):
        http = FakeHttp(err(400, "API key not valid."))
        out = LLMRepairer(provider=GeminiProvider(api_key="bad", model="m", http=http)).repair(BROKEN)
        self.assertIsNone(out.best)
        self.assertIn("rejected the API key", out.error)


if __name__ == "__main__":
    unittest.main()
