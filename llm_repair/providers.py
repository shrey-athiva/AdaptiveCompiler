"""LLM providers.

Every provider turns (system prompt, user prompt) into a list of raw candidate
dicts: ``{"fixed_source": ..., "explanation": ..., "confidence": ...}``.
Verification and ranking happen elsewhere (repairer.py), so providers are
interchangeable and the compiler stays the judge of every answer.

Providers:
    GeminiProvider     Google Gemini via plain HTTPS (no extra packages).
                       Free tier available: https://aistudio.google.com/apikey
    AnthropicProvider  Claude via the `anthropic` SDK (paid API).

Selection (make_provider): REPAIR_LLM_PROVIDER=gemini|anthropic, otherwise
whichever key is set (ANTHROPIC_API_KEY wins if both are).
"""

from __future__ import annotations

import json
import os
import time
import urllib.error
import urllib.request
from typing import Any, Callable, Dict, List, Optional, Tuple

TOOL_NAME = "propose_repairs"

ANTHROPIC_DEFAULT_MODEL = "claude-sonnet-5-5"

# Tried in order; a model that returns 404 (not available to this key) is skipped.
GEMINI_DEFAULT_MODELS = ["gemini-3.6-flash", "gemini-3.5-flash", "gemini-2.5-flash"]
GEMINI_BASE_URL = "https://generativelanguage.googleapis.com/v1beta/models"

# JSON Schema for one candidate list (used by both providers).
_CANDIDATES_SCHEMA: Dict[str, Any] = {
    "type": "object",
    "properties": {
        "candidates": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "fixed_source": {
                        "type": "string",
                        "description": "The complete corrected program.",
                    },
                    "explanation": {
                        "type": "string",
                        "description": "1-2 sentences: what was wrong and what changed.",
                    },
                    "confidence": {
                        "type": "number",
                        "description": "Your confidence from 0 to 1 that this matches the author's intent.",
                    },
                },
                "required": ["fixed_source", "explanation"],
            },
        }
    },
    "required": ["candidates"],
}

REPAIR_TOOL = {
    "name": TOOL_NAME,
    "description": (
        "Submit up to 3 candidate corrected versions of the program. Each "
        "candidate must be the COMPLETE corrected source file."
    ),
    "input_schema": _CANDIDATES_SCHEMA,
}


class LLMUnavailable(RuntimeError):
    """No usable provider (missing key, unknown provider, missing SDK)."""


class ProviderError(RuntimeError):
    """The provider was reached but the request failed (auth, quota, ...)."""


# --------------------------------------------------------------------------
# Shared parsing helpers
# --------------------------------------------------------------------------
def _candidates_from_text(text: str) -> List[dict]:
    """Pull {"candidates": [...]} out of free text / JSON text."""
    text = text.strip()
    for chunk in (text, text[text.find("{"): text.rfind("}") + 1]):
        if not chunk:
            continue
        try:
            data = json.loads(chunk)
        except ValueError:
            continue
        if isinstance(data, dict) and isinstance(data.get("candidates"), list):
            return [c for c in data["candidates"] if isinstance(c, dict)]
    return []


def _extract_candidates(response: Any) -> List[dict]:
    """Anthropic response: prefer the tool call, fall back to JSON in text."""
    texts: List[str] = []

    for block in getattr(response, "content", []) or []:
        btype = getattr(block, "type", None)
        if btype == "tool_use" and getattr(block, "name", "") == TOOL_NAME:
            data = getattr(block, "input", None)
            if isinstance(data, dict) and isinstance(data.get("candidates"), list):
                return [c for c in data["candidates"] if isinstance(c, dict)]
        elif btype == "text":
            texts.append(getattr(block, "text", ""))

    for text in texts:
        found = _candidates_from_text(text)
        if found:
            return found
    return []


# --------------------------------------------------------------------------
# Anthropic
# --------------------------------------------------------------------------
class AnthropicProvider:
    name = "anthropic"

    def __init__(self, client: Any = None, model: Optional[str] = None):
        self._client = client
        self.model = model or os.environ.get("REPAIR_LLM_MODEL", ANTHROPIC_DEFAULT_MODEL)

    @staticmethod
    def is_configured() -> bool:
        return bool(os.environ.get("ANTHROPIC_API_KEY"))

    def _get_client(self) -> Any:
        if self._client is not None:
            return self._client
        if not self.is_configured():
            raise LLMUnavailable(
                "ANTHROPIC_API_KEY is not set. Set it in the environment before running.")
        try:
            import anthropic
        except ImportError as exc:
            raise LLMUnavailable(
                "The 'anthropic' package is not installed. Run: pip install anthropic"
            ) from exc
        self._client = anthropic.Anthropic(timeout=60.0, max_retries=2)
        return self._client

    def propose(self, system: str, user: str) -> List[dict]:
        response = self._get_client().messages.create(
            model=self.model,
            max_tokens=4096,
            system=system,
            tools=[REPAIR_TOOL],
            # "auto" works on every model; forcing a tool is rejected by some.
            tool_choice={"type": "auto"},
            messages=[{"role": "user", "content": user}],
        )
        return _extract_candidates(response)


# --------------------------------------------------------------------------
# Gemini (plain HTTPS, standard library only)
# --------------------------------------------------------------------------
HttpFn = Callable[[str, str, Dict[str, str], Optional[dict], float], Tuple[int, dict]]


def _urllib_http(method: str, url: str, headers: Dict[str, str],
                 body: Optional[dict], timeout: float) -> Tuple[int, dict]:
    """Return (status, parsed JSON body). HTTP errors are returned, not raised."""
    data = json.dumps(body).encode("utf-8") if body is not None else None
    req = urllib.request.Request(
        url, data=data, method=method,
        headers={**headers, "Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req, timeout=timeout) as resp:
            return resp.status, json.loads(resp.read() or b"{}")
    except urllib.error.HTTPError as exc:
        try:
            return exc.code, json.loads(exc.read() or b"{}")
        except ValueError:
            return exc.code, {}
    except (urllib.error.URLError, OSError) as exc:
        reason = getattr(exc, "reason", exc)
        raise ProviderError(f"Could not reach the Gemini API: {reason}") from exc


class GeminiProvider:
    name = "gemini"
    RETRY_DELAY = 5.0   # seconds before the single retry on 429/503

    def __init__(self, api_key: Optional[str] = None, model: Optional[str] = None,
                 http: Optional[HttpFn] = None,
                 sleep: Callable[[float], None] = time.sleep):
        self._api_key = api_key
        chosen = model or os.environ.get("GEMINI_MODEL")
        self.models = [chosen] if chosen else list(GEMINI_DEFAULT_MODELS)
        self.model = self.models[0]
        self._http = http or _urllib_http
        self._sleep = sleep

    @staticmethod
    def _env_key() -> Optional[str]:
        return os.environ.get("GEMINI_API_KEY") or os.environ.get("GOOGLE_API_KEY")

    @classmethod
    def is_configured(cls) -> bool:
        return bool(cls._env_key())

    def _key(self) -> str:
        key = self._api_key or self._env_key()
        if not key:
            raise LLMUnavailable(
                "GEMINI_API_KEY is not set. Get a free key at "
                "https://aistudio.google.com/apikey and set it in the environment.")
        return key

    @staticmethod
    def _describe(status: int, data: dict) -> str:
        message = ""
        if isinstance(data, dict):
            message = str((data.get("error") or {}).get("message", "")).strip()
        lowered = message.lower()

        if status == 429:
            hint = ("Gemini rate limit or free-tier quota reached. "
                    "Wait a minute and try again.")
        elif status in (401, 403) or "api key" in lowered:
            hint = "Gemini rejected the API key (invalid, restricted, or not allowed here)."
        elif status == 503:
            hint = "Gemini is overloaded right now. Try again shortly."
        else:
            hint = f"Gemini API error {status}."
        return f"{hint} {message}".strip()

    @staticmethod
    def _text_of(data: dict) -> str:
        feedback = data.get("promptFeedback") or {}
        if feedback.get("blockReason"):
            raise ProviderError(f"Gemini blocked the request: {feedback['blockReason']}")

        parts: List[str] = []
        for cand in data.get("candidates") or []:
            for part in (cand.get("content") or {}).get("parts") or []:
                if part.get("thought"):      # skip reasoning summaries
                    continue
                if isinstance(part.get("text"), str):
                    parts.append(part["text"])
            if parts:
                break
        return "".join(parts)

    def propose(self, system: str, user: str) -> List[dict]:
        key = self._key()
        body = {
            "systemInstruction": {"parts": [{"text": system}]},
            "contents": [{"role": "user", "parts": [{"text": user}]}],
            "generationConfig": {
                "responseMimeType": "application/json",
                "responseJsonSchema": _CANDIDATES_SCHEMA,
                "maxOutputTokens": 8192,
            },
        }
        headers = {"x-goog-api-key": key}   # header, never the URL, so keys can't leak into logs

        unavailable: List[str] = []

        for model in self.models:
            url = f"{GEMINI_BASE_URL}/{model}:generateContent"

            for attempt in range(2):
                status, data = self._http("POST", url, headers, body, 60.0)
                if status in (429, 503) and attempt == 0:
                    self._sleep(self.RETRY_DELAY)
                    continue
                break

            if status == 200:
                self.model = model
                return _candidates_from_text(self._text_of(data))

            if status == 404:            # model not available to this key: try the next
                unavailable.append(model)
                continue

            raise ProviderError(self._describe(status, data))

        raise ProviderError(
            "None of these Gemini models were available to your key: "
            + ", ".join(unavailable)
            + ". Run `python repair_cli.py --list-models` and set GEMINI_MODEL.")

    def list_models(self) -> List[str]:
        """Models that support generateContent for this key."""
        status, data = self._http("GET", GEMINI_BASE_URL + "?pageSize=1000",
                                  {"x-goog-api-key": self._key()}, None, 30.0)
        if status != 200:
            raise ProviderError(self._describe(status, data))
        names = []
        for m in data.get("models", []):
            if "generateContent" in (m.get("supportedGenerationMethods") or []):
                names.append(str(m.get("name", "")).replace("models/", ""))
        return sorted(names)


# --------------------------------------------------------------------------
# Selection
# --------------------------------------------------------------------------
def any_provider_configured() -> bool:
    return AnthropicProvider.is_configured() or GeminiProvider.is_configured()


def make_provider(name: Optional[str] = None, model: Optional[str] = None):
    name = (name or os.environ.get("REPAIR_LLM_PROVIDER") or "").strip().lower()

    if name == "anthropic":
        return AnthropicProvider(model=model)
    if name == "gemini":
        return GeminiProvider(model=model)
    if name:
        raise LLMUnavailable(
            f"Unknown REPAIR_LLM_PROVIDER '{name}'. Use 'gemini' or 'anthropic'.")

    if AnthropicProvider.is_configured():
        return AnthropicProvider(model=model)
    if GeminiProvider.is_configured():
        return GeminiProvider(model=model)

    raise LLMUnavailable(
        "No LLM API key found. Set GEMINI_API_KEY (free key: "
        "https://aistudio.google.com/apikey) or ANTHROPIC_API_KEY.")
