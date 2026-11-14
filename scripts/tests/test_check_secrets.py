"""Testes do caçador de segredos, com um repositório falso injetado.

O scanner depende da abstração repo.Repository; aqui ela é implementada por
_FakeRepository, que devolve saídas de git prontas e usa uma pasta temporária
como raiz. Os "segredos" de teste são montados aos pedaços, pra este arquivo
não disparar o próprio caçador.
"""

# ------------------------------------------------------------------ Imports
import tempfile
import unittest
from pathlib import Path

from check_secrets import scanner

# ------------------------------------------- Constantes e variáveis privadas

_TOKEN = "gh" + "p_" + "a" * 36
_EMAIL = "fulano" + "@" + "exemplo.com.br"
_ALLOWED_EMAIL = "no" + "reply" + "@" + "example.com"
_PASSWORD_LINE = "sen" + "ha = 'abcd1234'"


class _FakeRepository:
    """Repositório falso: saídas de git prontas e uma raiz temporária."""

    def __init__(
        self, root: Path, staged: dict[str, list[str]], tracked: list[str]
    ) -> None:
        """Guarda o que vai fingir: arquivos no índice e versionados."""
        self._root = root
        self._staged = staged
        self._tracked = tracked

    def get_root(self) -> Path:
        """Devolve a raiz temporária."""
        return self._root

    def run_git(self, *args: str) -> str:
        """Responde aos comandos que o scanner usa."""
        if "ls-files" in args:
            return "\0".join(self._tracked) + "\0"
        if "--name-only" in args:
            return "\0".join(self._staged) + "\0"
        return "".join(
            _diff(path, lines) for path, lines in self._staged.items()
        )


# ---------------------------------------------------------------- Testes


class ScanStagedTest(unittest.TestCase):
    """O que vai pro próximo commit."""

    def setUp(self) -> None:
        """Cria uma raiz temporária vazia."""
        self._dir = tempfile.TemporaryDirectory()
        self.root = Path(self._dir.name)

    def tearDown(self) -> None:
        """Apaga a raiz temporária."""
        self._dir.cleanup()

    def test_finds_token_and_email(self) -> None:
        """Token e e-mail nas linhas adicionadas viram achados."""
        repository = _FakeRepository(
            self.root,
            {"src/config.py": [f"token = {_TOKEN}", "ok = 1", _EMAIL]},
            [],
        )
        rules = {(f.line, f.rule) for f in scanner.scan_staged(repository)}
        self.assertIn((1, "token do GitHub"), rules)
        self.assertIn((3, "e-mail"), rules)

    def test_ignore_marker_and_allowed_email(self) -> None:
        """Linha marcada e e-mail genérico passam."""
        repository = _FakeRepository(
            self.root,
            {
                "a.py": [
                    f"{_EMAIL}  # {scanner.IGNORE_MARKER}",
                    _ALLOWED_EMAIL,
                ]
            },
            [],
        )
        self.assertEqual(scanner.scan_staged(repository), [])

    def test_password_rule_skips_example_files(self) -> None:
        """Arquivo .example pode ter senha falsa; o de verdade, não."""
        repository = _FakeRepository(
            self.root,
            {
                "config.example.py": [_PASSWORD_LINE],
                "config.py": [_PASSWORD_LINE],
            },
            [],
        )
        paths = {f.path for f in scanner.scan_staged(repository)}
        self.assertEqual(paths, {"config.py"})

    def test_names_are_checked(self) -> None:
        """Nome de segredo e pasta de nota fiscal viram achados."""
        repository = _FakeRepository(
            self.root,
            {".env": ["X=1"], "docs/notas-fiscais/compra.pdf": ["..."]},
            [],
        )
        rules = {f.rule for f in scanner.scan_staged(repository)}
        self.assertIn("arquivo .env", rules)
        self.assertIn("pasta de nota fiscal", rules)

    def test_local_patterns_are_read_from_root(self) -> None:
        """Padrão pessoal sai do arquivo local, na raiz injetada."""
        local = self.root / scanner.LOCAL_PATTERNS_FILE
        local.parent.mkdir(parents=True)
        local.write_text("# comentário\ntartaruga\n", encoding="utf-8")
        repository = _FakeRepository(
            self.root, {"a.txt": ["a Tartaruga voadora"]}, []
        )
        rules = {f.rule for f in scanner.scan_staged(repository)}
        self.assertEqual(rules, {"padrão pessoal (local)"})


class ScanTrackedTest(unittest.TestCase):
    """Tudo que está versionado, lido do disco."""

    def test_reads_tracked_files_from_root(self) -> None:
        """Lê os arquivos versionados a partir da raiz injetada."""
        with tempfile.TemporaryDirectory() as name:
            root = Path(name)
            (root / "limpo.txt").write_text("nada aqui\n", encoding="utf-8")
            (root / "sujo.txt").write_text(f"oi\n{_EMAIL}\n", encoding="utf-8")
            repository = _FakeRepository(root, {}, ["limpo.txt", "sujo.txt"])
            findings = scanner.scan_tracked(repository)
        self.assertEqual(
            [(f.path, f.line, f.rule) for f in findings],
            [("sujo.txt", 2, "e-mail")],
        )


# ---------------------------------------------------------- Funções privadas


def _diff(path: str, lines: list[str]) -> str:
    """Monta o diff de um arquivo novo, como o git mostra."""
    added = "".join(f"+{line}\n" for line in lines)
    return (
        f"diff --git a/{path} b/{path}\n"
        f"--- /dev/null\n+++ b/{path}\n"
        f"@@ -0,0 +1,{len(lines)} @@\n{added}"
    )


# O disparo fica no fim: em Python, a função precisa existir antes de ser
# chamada.
if __name__ == "__main__":
    unittest.main()
