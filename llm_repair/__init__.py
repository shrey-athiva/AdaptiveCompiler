"""LLM-assisted repair on top of the Flex/Bison compiler.

Flow: compile -> (if broken) ask the LLM for candidate fixes -> recompile each
candidate with the real compiler -> keep only fixes the compiler accepts.
"""

from .compiler_runner import (
    CompileResult,
    Diagnostic,
    Repair,
    Symbol,
    CompilerNotFound,
    run_compiler,
)
from .providers import (
    AnthropicProvider,
    GeminiProvider,
    LLMUnavailable,
    ProviderError,
    make_provider,
)
from .repairer import (
    LLMRepairer,
    RepairCandidate,
    RepairOutcome,
    needs_llm,
)

__all__ = [
    "CompileResult", "Diagnostic", "Repair", "Symbol", "CompilerNotFound",
    "run_compiler", "LLMRepairer", "LLMUnavailable", "RepairCandidate",
    "RepairOutcome", "needs_llm", "AnthropicProvider", "GeminiProvider",
    "ProviderError", "make_provider",
]
