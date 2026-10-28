"""Regression tests for the compiler itself (needs the built binary: `make`)."""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from llm_repair import run_compiler  # noqa: E402


def prog(*lines):
    return "int main() {\n" + "\n".join(lines) + "\n}\n"


def codes(result):
    return [d.code for d in result.diagnostics]


class CleanPrograms(unittest.TestCase):
    def test_valid_program_is_clean(self):
        r = run_compiler(prog("int a = 3;", "float c = a * 2 + 1;",
                              "while (a < 10) {", "a = a + 1;", "}"))
        self.assertEqual(r.result, "clean")
        self.assertEqual(r.returncode, 0)
        self.assertIn("ASSIGN c", r.ir)  # IR produced

    def test_if_else_ir(self):
        r = run_compiler(prog("int x = 1;", "if (x > 0) {", "x = 2;", "} else {",
                              "x = 3;", "}"))
        self.assertTrue(r.ok)
        self.assertIn("IF x > 0 GOTO L1", r.ir)

    def test_use_before_init_is_warning_only(self):
        r = run_compiler(prog("int x;", "int y = x + 1;"))
        self.assertEqual(r.result, "warnings")
        self.assertTrue(r.ok)


class SyntaxRecovery(unittest.TestCase):
    def test_missing_semicolon_marker_and_confidence(self):
        r = run_compiler(prog("int x = 5", "x = x + 1;"))
        self.assertEqual(r.result, "recovered")
        self.assertEqual(r.returncode, 0)
        self.assertEqual(r.repairs[0].kind, "ADD_SEMICOLON")
        self.assertEqual(r.repairs[0].args, ["2"])
        self.assertAlmostEqual(r.repairs[0].confidence, 0.95)

    def test_missing_semicolon_on_char_declaration(self):
        r = run_compiler(prog("char c = 'a'"))
        self.assertEqual(r.repairs[0].kind, "ADD_SEMICOLON")

    def test_missing_rparen_now_emits_marker(self):
        # Regression: REPAIR:ADD_RPAREN was never printed, so the UI never offered it.
        r = run_compiler(prog("int x = 5;", "if (x >= 5 {", "x = x + 1;", "}"))
        self.assertEqual(r.result, "recovered")
        self.assertEqual([x.kind for x in r.repairs], ["ADD_RPAREN"])
        self.assertEqual(r.repairs[0].args, ["3"])

    def test_no_trailing_newline(self):
        r = run_compiler("int main() {\nint x = 1\n}")
        self.assertEqual(r.result, "recovered")


class SkippedStatements(unittest.TestCase):
    def test_skipped_statement_leaves_no_ir(self):
        # Regression: 'ADD 1 2 -> t1' leaked from the discarded statement.
        r = run_compiler(prog("int x = 1;", "x = 1 + 2 +;", "x = 3;"))
        self.assertEqual(r.result, "syntax_error")
        self.assertEqual(r.returncode, 1)          # was 0: silently dropped code
        self.assertNotIn("ADD", r.ir)
        self.assertIn("ASSIGN x 3", r.ir)           # parsing continued

    def test_recovery_inside_if_body_keeps_outer_if_ir(self):
        r = run_compiler(prog("int x = 1;", "if (x > 0) {", "x = 2 +;", "x = 3;",
                              "}", "x = 4;"))
        self.assertIn("IF x > 0 GOTO L1", r.ir)
        self.assertIn("ASSIGN x 3", r.ir)
        self.assertIn("ASSIGN x 4", r.ir)
        self.assertNotIn("ASSIGN x 2", r.ir)

    def test_syntax_error_has_line_and_token(self):
        r = run_compiler(prog("int x = 1;", "x = 1 + 2 +;"))
        self.assertIn("Syntax Error at line 3", r.report)
        self.assertIn("unexpected SEMICOLON", r.report)

    def test_lexical_error_reported_and_fails(self):
        r = run_compiler(prog("int x = 5 @ 3;"))
        self.assertEqual(r.result, "lexical_error")
        self.assertIn("LEXICAL", codes(r))


class SemanticRepair(unittest.TestCase):
    def test_transposition_typo_scores_high(self):
        r = run_compiler(prog("int count = 5;", "cuont = 7;"))
        self.assertEqual(r.repairs[0].kind, "REPLACE_IDENTIFIER")
        self.assertEqual(r.repairs[0].args, ["cuont", "count"])
        self.assertGreaterEqual(r.repairs[0].confidence, 0.85)

    def test_char_typo_now_gets_candidate(self):
        # Regression: type bonus only covered int/float, so char typos got none.
        r = run_compiler(prog("char letter = 'a';", "letr = 'b';"))
        self.assertEqual(r.repairs[0].kind, "REPLACE_IDENTIFIER")
        self.assertEqual(r.repairs[0].args, ["letr", "letter"])

    def test_typo_inside_expression_is_repaired_not_declared(self):
        # Regression: with no type context the score capped at 0.50, so a typo
        # in an expression fell through to "declare a new variable".
        r = run_compiler(prog("int count = 5;", "int marks = 85;",
                              "int total = cuont + marks;"))
        self.assertEqual(r.repairs[0].kind, "REPLACE_IDENTIFIER")
        self.assertEqual(r.repairs[0].args, ["cuont", "count"])
        self.assertLess(r.repairs[0].confidence, 0.85)   # never High without type evidence
        self.assertGreaterEqual(r.repairs[0].confidence, 0.70)

    def test_unrelated_name_in_expression_still_declares(self):
        r = run_compiler(prog("int count = 5;", "int total = zzz + count;"))
        self.assertEqual(r.repairs[0].kind, "DECLARE_VARIABLE")

    def test_short_dissimilar_names_are_not_confused(self):
        r = run_compiler(prog("int x = 1;", "int y = z + x;"))
        self.assertEqual(r.repairs[0].kind, "DECLARE_VARIABLE")

    def test_unrelated_name_proposes_declaration(self):
        r = run_compiler(prog("int count = 5;", "zzz = 7;"))
        self.assertEqual(r.repairs[0].kind, "DECLARE_VARIABLE")

    def test_float_declaration_inferred_from_usage(self):
        r = run_compiler(prog("float f = 1.5;", "qqq = 2.5;"))
        self.assertEqual(r.repairs[0].args[:2], ["qqq", "float"])

    def test_one_mistake_one_diagnostic(self):
        # Regression: one undeclared variable used to produce three errors.
        r = run_compiler(prog("int x = y + 1;"))
        self.assertEqual(codes(r), ["UNDECLARED"])
        self.assertEqual(r.errors, 1)

    def test_type_mismatch_has_concrete_suggestion(self):
        r = run_compiler(prog("int x = 2.5;"))
        self.assertEqual(codes(r), ["TYPE_MISMATCH"])
        self.assertIn("float", r.report)

    def test_bad_comparison_reported_once(self):
        r = run_compiler(prog("int x = 1;", "char c = 'a';", "if (x < c) {",
                              "x = 2;", "}"))
        self.assertEqual(codes(r), ["BAD_COMPARISON"])

    def test_duplicate_declaration(self):
        r = run_compiler(prog("int x;", "int x;"))
        self.assertIn("DUPLICATE_DECLARATION", codes(r))

    def test_symbols_marker(self):
        r = run_compiler(prog("int a = 1;", "float b;"))
        self.assertEqual([(s.name, s.type, s.initialized) for s in r.symbols],
                         [("a", "int", True), ("b", "float", False)])


if __name__ == "__main__":
    unittest.main()
