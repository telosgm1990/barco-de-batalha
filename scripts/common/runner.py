"""Rodar um programa de fora (docker, ruff, kicad-cli) e pegar o resultado.

Quem precisa rodar comando recebe um CommandRunner por parâmetro (injeção de
dependência): o de verdade é o SubprocessRunner; nos testes, um falso que
devolve o resultado combinado, sem rodar nada.
"""

# ------------------------------------------------------------------ Imports
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Protocol

# ------------------------------------------------------------ Tipos públicos


@dataclass(frozen=True)
class CommandResult:
    """O que um comando devolveu: o código de saída e o texto que escreveu."""

    returncode: int
    output: str  # saída e erro juntos


class CommandRunner(Protocol):
    """Quem roda comandos pros scripts."""

    def run(self, command: list[str], cwd: Path) -> CommandResult:
        """Roda o comando na pasta e espera ele terminar."""
        ...


class SubprocessRunner:
    """Roda de verdade, com o subprocess da biblioteca padrão."""

    def run(self, command: list[str], cwd: Path) -> CommandResult:
        """Roda o comando na pasta e devolve o código e a saída, em texto.

        Programa que não existe vira código 127 (o mesmo do shell), em vez de
        exceção: pra quem chama, é só mais um comando que falhou.
        """
        try:
            result = subprocess.run(
                command,
                cwd=cwd,
                capture_output=True,
                check=False,  # Quem chama decide o que é falha
            )
        except FileNotFoundError:
            return CommandResult(127, f"programa não encontrado: {command[0]}")
        output = (result.stdout + result.stderr).decode("utf-8", "replace")
        return CommandResult(result.returncode, output)


# ------------------------------------------------------------- API pública

__all__ = ["CommandResult", "CommandRunner", "SubprocessRunner"]
