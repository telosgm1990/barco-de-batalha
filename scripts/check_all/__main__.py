"""Linha de comando do check-all.

Uso, com o .venv de `scripts/` criado:

    check-all            # tudo, inclusive o que roda no Docker (demora)
    check-all --quick    # sem Docker: só as rápidas

Sem o .venv, a partir da pasta `scripts` (só usa biblioteca padrão; o que
falta na máquina é pulado):

    python -m check_all [--quick]

Saída: 0 se nada falhou (pulada não é falha), 1 se alguma falhou.
"""

# ------------------------------------------------------------------ Imports
import argparse
import sys

from common import repo, runner

from . import checks

# ------------------------------------------- Constantes e variáveis privadas

_ICONS = {"ok": "✅", "falhou": "❌", "pulado": "⏭️"}

# ---------------------------------------------------------- Ponto de entrada


def main(argv: list[str] | None = None) -> int:
    """Monta as peças, roda as checagens e devolve o código de saída."""
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    parser = argparse.ArgumentParser(
        prog="check-all",
        description="Roda todas as checagens do repositório.",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="pula o que roda no Docker (compilar, testes do firmware, Doxygen)",
    )
    args = parser.parse_args(argv)

    # O ponto de montagem: as peças de verdade nascem aqui
    repository = repo.GitRepository()
    root = repository.get_root()
    cpp_files = repository.run_git(
        "ls-files", "software/firmware/*.cpp", "software/firmware/*.hpp"
    ).split()
    to_run = checks.build_checks(root, cpp_files, with_docker=not args.quick)

    print(f"check-all: {len(to_run)} checagens")
    outcomes = checks.run_checks(
        to_run,
        runner.SubprocessRunner(),
        checks.SystemToolbox(),
        on_outcome=_print_outcome,
    )
    _print_summary(outcomes)
    return checks.exit_code(outcomes)


# ---------------------------------------------------------- Funções privadas


def _print_outcome(outcome: checks.Outcome) -> None:
    """Mostra uma checagem que terminou, com o motivo ou o fim da saída."""
    icon = _ICONS[outcome.status]
    if outcome.status == "pulado":
        print(f"  {icon} {outcome.check.name} — {outcome.detail}")
        return
    print(f"  {icon} {outcome.check.name} ({outcome.seconds:.0f} s)")
    for line in outcome.detail.splitlines():
        print(f"       {line}")
    sys.stdout.flush()


def _print_summary(outcomes: list[checks.Outcome]) -> None:
    """Mostra quantas passaram, falharam e foram puladas."""
    counts = {status: 0 for status in _ICONS}
    for outcome in outcomes:
        counts[outcome.status] += 1
    print(
        f"Resultado: {counts['ok']} ok, {counts['falhou']} falhou, "
        f"{counts['pulado']} pulado"
    )


if __name__ == "__main__":
    sys.exit(main())
