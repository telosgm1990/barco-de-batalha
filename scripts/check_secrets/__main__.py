"""Linha de comando do caçador de segredos.

Uso, com o .venv de `scripts/` criado:

    check-secrets            # o que está no índice (o próximo commit)
    check-secrets --all      # tudo que está versionado

Sem o .venv, a partir da pasta `scripts` (só usa biblioteca padrão):

    python -m check_secrets [--all]

Saída: 0 se está limpo, 1 se achou algo, 2 se não deu pra conferir.
"""

# ------------------------------------------------------------------ Imports
import argparse
import re
import sys

from . import scanner

# ---------------------------------------------------------- Ponto de entrada


def main(argv: list[str] | None = None) -> int:
    """Roda a conferência e devolve o código de saída."""
    # No Windows, saída em pipe (hook do git) sai em cp1252 sem isto.
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    sys.stderr.reconfigure(encoding="utf-8", errors="replace")

    parser = argparse.ArgumentParser(
        prog="check-secrets",
        description="Caçador de segredos do repositório.",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="confere todos os arquivos versionados, não só o índice",
    )
    args = parser.parse_args(argv)

    try:
        findings = scanner.scan_tracked() if args.all else scanner.scan_staged()
    except (RuntimeError, re.error) as error:
        print(
            f"Caçador de segredos: não deu pra conferir — {error}",
            file=sys.stderr,
        )
        return 2

    _report(findings, "no repositório" if args.all else "no que vai pro commit")
    return 1 if findings else 0


# ---------------------------------------------------------- Funções privadas


def _report(findings: list[scanner.Finding], where: str) -> None:
    """Imprime o resultado pra pessoa."""
    if not findings:
        print(f"Caçador de segredos: nada suspeito {where}.")
        return
    print(f"Caçador de segredos: {len(findings)} achado(s) {where}.\n")
    for finding in findings:
        place = finding.path
        if finding.line:
            place += f":{finding.line}"
        print(f"  {place}  [{finding.rule}]  {finding.excerpt}")
    print(
        "\nSegredo guardado é segredo não commitado. Tire do arquivo (ou ponha"
        "\nno .gitignore) e tente de novo. Falso alarme? Marque a linha com"
        f'\n"{scanner.IGNORE_MARKER}".'
    )


# O disparo fica no fim: em Python, a função precisa existir antes de ser
# chamada, então o `if __name__` não pode subir junto com o main().
if __name__ == "__main__":
    sys.exit(main())
