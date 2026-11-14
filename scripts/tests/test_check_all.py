"""Testes do check-all, com peças falsas: nada roda de verdade."""

# ------------------------------------------------------------------ Imports
import unittest
from pathlib import Path

from check_all import checks
from common.runner import CommandResult

# ------------------------------------------------------------ Tipos públicos


class _FakeRunner:
    """Finge rodar: falha o comando que tiver algum pedaço em `failing`."""

    def __init__(self, failing: tuple[str, ...] = ()) -> None:
        self.failing = failing
        self.commands: list[list[str]] = []

    def run(self, command: list[str], cwd: Path) -> CommandResult:
        self.commands.append(command)
        if any(part in arg for part in self.failing for arg in command):
            return CommandResult(1, "linha 1\nalgo deu errado\n")
        return CommandResult(0, "tudo certo\n")


class _FakeToolbox:
    """Só tem as ferramentas da lista, num caminho de mentira."""

    def __init__(self, tools: tuple[str, ...]) -> None:
        self.tools = tools

    def find(self, name: str) -> str | None:
        return f"/fake/{name}" if name in self.tools else None


class CheckAllTest(unittest.TestCase):
    """O que o check-all monta e como ele decide."""

    _ALL_TOOLS = ("ruff", "clang-format", "kicad-cli", "docker")

    def setUp(self) -> None:
        """Monta a lista completa, com dois arquivos de C++."""
        self.root = Path("/repo")
        self.list = checks.build_checks(self.root, ["a.cpp", "b.hpp"])

    def _run(self, runner, tools=_ALL_TOOLS):
        return checks.run_checks(
            self.list, runner, _FakeToolbox(tools), clock=lambda: 0.0
        )

    def test_everything_passes(self) -> None:
        """Tudo passando: tudo ok e saída 0."""
        outcomes = self._run(_FakeRunner())
        self.assertTrue(all(o.status == "ok" for o in outcomes))
        self.assertEqual(checks.exit_code(outcomes), 0)

    def test_missing_tool_is_skipped_not_failed(self) -> None:
        """Ferramenta que falta pula a checagem, sem falhar."""
        outcomes = self._run(_FakeRunner(), tools=("ruff", "docker"))
        skipped = {o.check.name for o in outcomes if o.status == "pulado"}
        self.assertEqual(skipped, {"clang-format", "ERC do esquemático"})
        self.assertEqual(checks.exit_code(outcomes), 0)

    def test_closed_docker_skips_all_docker_checks(self) -> None:
        """Docker fechado pula as três do Docker, conferindo uma vez só."""
        runner = _FakeRunner(failing=("info",))
        outcomes = self._run(runner)
        docker = [o for o in outcomes if o.check.needs == "docker"]
        self.assertEqual(len(docker), 3)
        self.assertTrue(all(o.status == "pulado" for o in docker))
        # Conferiu o Docker uma vez só
        infos = [c for c in runner.commands if c[-1] == "info"]
        self.assertEqual(len(infos), 1)

    def test_failure_shows_the_end_of_the_output(self) -> None:
        """Falha mostra o fim da saída e dá saída 1."""
        outcomes = self._run(_FakeRunner(failing=("--check",)))
        failed = [o for o in outcomes if o.status == "falhou"]
        self.assertEqual([o.check.name for o in failed], ["ruff: formatação"])
        self.assertIn("algo deu errado", failed[0].detail)
        self.assertEqual(checks.exit_code(outcomes), 1)

    def test_tool_path_replaces_the_command_name(self) -> None:
        """O caminho achado da ferramenta entra no lugar do nome."""
        runner = _FakeRunner()
        self._run(runner)
        clang = [c for c in runner.commands if c[0].endswith("clang-format")]
        self.assertEqual(clang[0][0], "/fake/clang-format")
        self.assertEqual(clang[0][-2:], ["a.cpp", "b.hpp"])

    def test_quick_leaves_docker_out(self) -> None:
        """O --quick deixa as três do Docker de fora."""
        quick = checks.build_checks(self.root, [], with_docker=False)
        self.assertFalse(any(c.needs == "docker" for c in quick))
        self.assertEqual(len(quick), len(self.list) - 3)


class KicadVersionTest(unittest.TestCase):
    """A versão do KiCad sai da pasta, em números."""

    def test_ten_wins_over_seven(self) -> None:
        """A 10.0 é mais nova que a 7.0, mesmo vindo antes em texto."""
        paths = [
            "C:/Program Files/KiCad/10.0/bin/kicad-cli.exe",
            "C:/Program Files/KiCad/7.0/bin/kicad-cli.exe",
        ]
        newest = max(paths, key=checks._kicad_version)
        self.assertIn("10.0", newest)


if __name__ == "__main__":
    unittest.main()
