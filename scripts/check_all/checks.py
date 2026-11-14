"""As checagens do repositório e quem roda cada uma.

Uma checagem é só dado: um nome, um comando, a pasta onde ele roda e a
ferramenta que precisa estar na máquina. Quem roda é o run_checks, que
recebe por parâmetro quem executa os comandos (CommandRunner) e quem acha as
ferramentas (Toolbox) — nos testes, peças falsas.
"""

# ------------------------------------------------------------------ Imports
import glob
import os
import shutil
import sys
import tempfile
import time
from collections.abc import Callable
from dataclasses import dataclass
from pathlib import Path
from typing import Protocol

from common.runner import CommandRunner

# ------------------------------------------- Constantes de configuração

# A mesma imagem do firmware (D9): trocar a versão é decisão, não detalhe
IDF_IMAGE = "espressif/idf:v6.1"

# Linhas da saída mostradas quando uma checagem falha
FAILURE_TAIL_LINES = 15

# ------------------------------------------------------------ Tipos públicos


@dataclass(frozen=True)
class Check:
    """Uma checagem: um comando, rodado numa pasta do repositório."""

    name: str
    command: tuple[str, ...]
    cwd: Path
    # Ferramenta que precisa existir na máquina; o comando começa com ela
    needs: str | None = None


@dataclass(frozen=True)
class Outcome:
    """Como uma checagem terminou."""

    check: Check
    status: str  # "ok", "falhou" ou "pulado"
    detail: str  # o fim da saída, se falhou; o motivo, se pulou
    seconds: float


class Toolbox(Protocol):
    """Quem acha as ferramentas instaladas na máquina."""

    def find(self, name: str) -> str | None:
        """Devolve o caminho do executável, ou None se não tiver."""
        ...


class SystemToolbox:
    """Acha as ferramentas no .venv, no PATH e onde o KiCad se instala."""

    def find(self, name: str) -> str | None:
        """Devolve o caminho do executável, ou None se não tiver.

        Procura primeiro ao lado do Python do .venv (ruff, clang-format), depois
        no PATH. O kicad-cli não costuma estar no PATH: vale a variável
        KICAD_CLI ou a pasta padrão de instalação do KiCad no Windows.
        """
        venv_bin = str(Path(sys.executable).parent)
        path = os.pathsep.join([venv_bin, os.environ.get("PATH", "")])
        found = shutil.which(name, path=path)
        if found is not None or name != "kicad-cli":
            return found
        if os.environ.get("KICAD_CLI"):
            return os.environ["KICAD_CLI"]
        installed = sorted(glob.glob(_KICAD_WINDOWS_GLOB), key=_kicad_version)
        return installed[-1] if installed else None


# ------------------------------------------- Constantes e variáveis privadas

# Onde o instalador do KiCad põe o kicad-cli no Windows. Pode ter mais de uma
# versão (a 7 convive com a 10 neste PC): a mais nova ganha
_KICAD_WINDOWS_GLOB = r"C:\Program Files\KiCad\*\bin\kicad-cli.exe"

# ------------------------------------------------------------- API pública

__all__ = [
    "IDF_IMAGE",
    "FAILURE_TAIL_LINES",
    "Check",
    "Outcome",
    "Toolbox",
    "SystemToolbox",
    "build_checks",
    "run_checks",
    "exit_code",
]


def build_checks(
    root: Path, cpp_files: list[str], with_docker: bool = True
) -> list[Check]:
    """Monta a lista de checagens, das rápidas pras demoradas.

    Args:
        root: raiz do repositório.
        cpp_files: os .cpp e .hpp do firmware, relativos à raiz.
        with_docker: false pula as que rodam no Docker (compilar, testes do
            firmware e Doxygen), que são as demoradas.
    """
    python = sys.executable
    scripts = root / "scripts"
    firmware = root / "software" / "firmware"
    erc_report = Path(tempfile.gettempdir()) / "donatelos-erc.rpt"
    checks = [
        Check(
            "caçador de segredos",
            (python, "-m", "check_secrets", "--all"),
            scripts,
        ),
        Check(
            "testes do Python",
            (python, "-m", "unittest", "discover", "-s", "tests", "-t", "."),
            scripts,
        ),
        Check("ruff: lint", ("ruff", "check", "."), scripts, "ruff"),
        Check(
            "ruff: formatação",
            ("ruff", "format", "--check", "."),
            scripts,
            "ruff",
        ),
        Check(
            "clang-format",
            (
                "clang-format",
                "--dry-run",
                "--Werror",
                "--style=file:software/firmware/.clang-format",
                *cpp_files,
            ),
            root,
            "clang-format",
        ),
        Check(
            "ERC do esquemático",
            (
                "kicad-cli",
                "sch",
                "erc",
                "--exit-code-violations",
                "--output",
                str(erc_report),
                "hardware/kicad/donatelos.kicad_sch",
            ),
            root,
            "kicad-cli",
        ),
    ]
    if not with_docker:
        return checks
    return checks + [
        Check(
            "testes do firmware no PC",
            _in_idf(
                firmware,
                "/project/tests",
                "cmake -S . -B build -G Ninja && cmake --build build "
                "&& ./build/host_tests",
            ),
            root,
            "docker",
        ),
        Check(
            "compilar o firmware",
            _in_idf(firmware, "/project", "idf.py build"),
            root,
            "docker",
        ),
        Check(
            "Doxygen sem aviso",
            _in_idf(
                firmware,
                "/project",
                "apt-get update -qq && apt-get install -y -qq doxygen "
                "> /dev/null && rm -rf build/doxygen && mkdir -p "
                "build/doxygen && doxygen && test ! -s "
                "build/doxygen/avisos.txt || (cat build/doxygen/avisos.txt; "
                "exit 1)",
            ),
            root,
            "docker",
        ),
    ]


def run_checks(
    checks: list[Check],
    runner: CommandRunner,
    toolbox: Toolbox,
    clock: Callable[[], float] = time.monotonic,
    on_outcome: Callable[[Outcome], None] | None = None,
) -> list[Outcome]:
    """Roda as checagens em ordem e devolve como cada uma terminou.

    Ferramenta que não está na máquina pula a checagem, não falha. O Docker
    é conferido uma vez só, antes da primeira checagem que precisa dele: com
    o Docker Desktop fechado, todas as do Docker pulam.

    Args:
        checks: o que rodar, na ordem.
        runner: quem executa os comandos.
        toolbox: quem acha as ferramentas.
        clock: relógio em segundos, pra medir quanto cada uma levou.
        on_outcome: chamado a cada checagem que termina, pra mostrar já.
    """
    outcomes = []
    docker_problem: str | None = None
    docker_checked = False
    for check in checks:
        start = clock()
        tool = toolbox.find(check.needs) if check.needs else None
        if check.needs and tool is None:
            outcome = Outcome(
                check, "pulado", f"{check.needs} não encontrado", 0
            )
        else:
            if check.needs == "docker" and not docker_checked:
                docker_checked = True
                docker_problem = _docker_problem(runner, tool, check.cwd)
            if check.needs == "docker" and docker_problem:
                outcome = Outcome(check, "pulado", docker_problem, 0)
            else:
                command = [tool, *check.command[1:]] if tool else check.command
                result = runner.run(list(command), check.cwd)
                ok = result.returncode == 0
                outcome = Outcome(
                    check,
                    "ok" if ok else "falhou",
                    "" if ok else _tail(result.output),
                    clock() - start,
                )
        outcomes.append(outcome)
        if on_outcome is not None:
            on_outcome(outcome)
    return outcomes


def exit_code(outcomes: list[Outcome]) -> int:
    """Devolve 1 se alguma checagem falhou; pulada não conta como falha."""
    return 1 if any(o.status == "falhou" for o in outcomes) else 0


# ---------------------------------------------------------- Funções privadas


def _in_idf(firmware: Path, workdir: str, script: str) -> tuple[str, ...]:
    """Comando que roda o script no Docker, com o firmware em /project."""
    return (
        "docker",
        "run",
        "--rm",
        "-v",
        f"{firmware}:/project",
        "-w",
        workdir,
        IDF_IMAGE,
        "bash",
        "-c",
        script,
    )


def _docker_problem(
    runner: CommandRunner, docker: str, cwd: Path
) -> str | None:
    """Diz por que o Docker não serve agora, ou None se está pronto."""
    if runner.run([docker, "info"], cwd).returncode != 0:
        return "Docker Desktop fechado: abra e rode de novo"
    return None


def _kicad_version(path: str) -> tuple[int, ...]:
    """A versão do KiCad pela pasta (`.../KiCad/10.0/bin/...`), em números.

    Comparar em texto poria a 7.0 depois da 10.0.
    """
    folder = Path(path).parents[1].name
    return tuple(int(part) for part in folder.split(".") if part.isdigit())


def _tail(output: str) -> str:
    """As últimas linhas não vazias da saída."""
    lines = [line for line in output.splitlines() if line.strip()]
    return "\n".join(lines[-FAILURE_TAIL_LINES:])
