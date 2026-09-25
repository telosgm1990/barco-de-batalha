"""O repositório visto pelos scripts: onde fica a raiz e como falar com o git.

Script aqui trabalha com caminho relativo à raiz do repositório, nunca com
caminho absoluto de máquina. É este módulo que diz onde a raiz está.
"""

# ------------------------------------------------------------------ Imports
import subprocess
from pathlib import Path

# ------------------------------------------- Constantes e variáveis privadas

# Este arquivo mora em <raiz>/scripts/common/repo.py. O .venv instala os
# scripts em modo editável, então o caminho aponta sempre pro repositório.
_ROOT = Path(__file__).resolve().parents[2]

# ------------------------------------------------------------- API pública

__all__ = ["get_root", "run_git"]


def get_root() -> Path:
    """Devolve a raiz do repositório, independente da pasta atual."""
    return _ROOT


def run_git(*args: str) -> str:
    """Roda um comando git na raiz do repositório e devolve a saída em texto.

    Caminho com acento sai legível (`core.quotepath=off`).

    Levanta RuntimeError se o git terminar com erro.
    """
    result = subprocess.run(
        ["git", "-c", "core.quotepath=off", *args],
        cwd=_ROOT,
        capture_output=True,
        check=False,  # O erro vira RuntimeError logo abaixo, com a mensagem
    )
    if result.returncode != 0:
        message = result.stderr.decode("utf-8", "replace").strip()
        raise RuntimeError(message or f"git {args[0]} falhou")
    return result.stdout.decode("utf-8", "replace")
