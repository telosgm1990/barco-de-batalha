"""O repositório visto pelos scripts: onde fica a raiz e como falar com o git.

Quem precisa do repositório recebe um, por parâmetro (injeção de
dependência): o de verdade é o GitRepository; nos testes, um falso com a
mesma cara. Script aqui trabalha com caminho relativo à raiz do repositório,
nunca com caminho absoluto de máquina.
"""

# ------------------------------------------------------------------ Imports
import subprocess
from pathlib import Path
from typing import Protocol

# ------------------------------------------------------------ Tipos públicos


class Repository(Protocol):
    """O que os scripts precisam de um repositório."""

    def get_root(self) -> Path:
        """Devolve a raiz do repositório."""
        ...

    def run_git(self, *args: str) -> str:
        """Roda um comando git na raiz e devolve a saída em texto."""
        ...


class GitRepository:
    """Este checkout, com o git instalado na máquina."""

    def __init__(self, root: Path | None = None) -> None:
        """Guarda a raiz; sem ela, usa a do checkout onde o script mora."""
        self._root = root if root is not None else _ROOT

    def get_root(self) -> Path:
        """Devolve a raiz do repositório, independente da pasta atual."""
        return self._root

    def run_git(self, *args: str) -> str:
        """Roda um comando git na raiz do repositório e devolve a saída.

        Caminho com acento sai legível (`core.quotepath=off`).

        Levanta RuntimeError se o git terminar com erro.
        """
        result = subprocess.run(
            ["git", "-c", "core.quotepath=off", *args],
            cwd=self._root,
            capture_output=True,
            check=False,  # O erro vira RuntimeError logo abaixo, com a mensagem
        )
        if result.returncode != 0:
            message = result.stderr.decode("utf-8", "replace").strip()
            raise RuntimeError(message or f"git {args[0]} falhou")
        return result.stdout.decode("utf-8", "replace")


# ------------------------------------------- Constantes e variáveis privadas

# Este arquivo mora em <raiz>/scripts/common/repo.py. O .venv instala os
# scripts em modo editável, então o caminho aponta sempre pro repositório.
_ROOT = Path(__file__).resolve().parents[2]

# ------------------------------------------------------------- API pública

__all__ = ["Repository", "GitRepository"]
