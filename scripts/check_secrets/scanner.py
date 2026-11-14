"""As regras do caçador de segredos e a varredura do repositório.

Duas varreduras: a do índice (só as linhas adicionadas, é o que vai pro
próximo commit) e a de tudo que está versionado (arquivo inteiro, como está no
disco). Nas duas, confere o nome de cada arquivo e o conteúdo linha a linha.

O repositório chega por parâmetro, como abstração (repo.Repository): a
linha de comando passa o de verdade, os testes passam um falso.
"""

# ------------------------------------------------------------------ Imports
import re
from collections.abc import Iterator
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

from common import repo

# ------------------------------------------- Constantes de configuração

LOCAL_PATTERNS_FILE = "scripts/secret-patterns.local.txt"
IGNORE_MARKER = "check-secrets: ignore"
MAX_FILE_BYTES = 2_000_000

# ------------------------------------------------------------ Tipos públicos


@dataclass(frozen=True)
class Finding:
    """Um achado: onde está e qual regra casou."""

    path: str
    line: int  # 0 quando o problema é o nome do arquivo
    rule: str
    excerpt: str


# ------------------------------------------- Constantes e variáveis privadas

_Rules = list[tuple[str, re.Pattern[str]]]

_CONTENT_RULES: _Rules = [
    ("chave privada", re.compile(r"-----BEGIN [A-Z ]*PRIVATE KEY-----")),
    (
        "token do GitHub",
        re.compile(r"\b(gh[pousr]_[A-Za-z0-9]{36}|github_pat_\w{22,})\b"),
    ),
    ("chave de API (sk-)", re.compile(r"\bsk-(ant-)?[A-Za-z0-9_-]{20,}")),
    ("chave da AWS", re.compile(r"\bAKIA[0-9A-Z]{16}\b")),
    ("chave do Google", re.compile(r"\bAIza[0-9A-Za-z_-]{35}\b")),
    ("token do Slack", re.compile(r"\bxox[abprs]-[A-Za-z0-9-]{10,}")),
    (
        "senha no código",
        re.compile(
            r"(?i)\b\w*(pass(word)?|passwd|senha|secret|token|api_?key)\w*\b"
            r"[\"']?\s*[:=]\s*[\"'][^\"'\s]{4,}[\"']"
        ),
    ),
    (
        "senha em #define",
        re.compile(
            r"(?i)#define\s+\w*(pass|senha|secret|token|key)\w*\s+"
            r"\"[^\"]{4,}\""
        ),
    ),
    (
        "caminho de usuário (Windows)",
        re.compile(r"(?i)\b[a-z]:[\\/]+users[\\/]+[^\\/\s\"'<>]+"),
    ),
    (
        "caminho de usuário (Unix)",
        re.compile(r"(?i)(?<![\w.])/(home|users)/[a-z0-9._-]+"),
    ),
    (
        "e-mail",
        re.compile(r"\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b"),
    ),
    ("CPF", re.compile(r"\b\d{3}\.\d{3}\.\d{3}-\d{2}\b")),
]

_NAME_RULES: _Rules = [
    ("arquivo .env", re.compile(r"^\.env(\..+)?$")),
    (
        "chave ou certificado",
        re.compile(r"\.(key|pem|p12|pfx)$", re.IGNORECASE),
    ),
    ("chave SSH", re.compile(r"(^|_)(rsa|dsa|ecdsa|ed25519)(\.pub)?$")),
    (
        "arquivo de segredos",
        re.compile(r"^(secrets\..+|credentials.*\.json)$", re.IGNORECASE),
    ),
]

# Arquivo de exemplo leva valor falso de propósito: as regras de senha não
# valem nele. As outras (caminho, e-mail, chave de verdade) valem.
_PASSWORD_RULES = frozenset({"senha no código", "senha em #define"})

_FORBIDDEN_DIRS = frozenset({"notas-fiscais", "comprovantes"})

# Local e domínio separados, pra este arquivo não ter um e-mail inteiro.
_EMAIL_ALLOWED_USERS = frozenset({"git", "noreply"})
_EMAIL_ALLOWED_DOMAINS = frozenset(
    {"example.com", "example.org", "users.noreply.github.com"}
)

_HUNK_HEADER = re.compile(r"^@@ -\S+ \+(\d+)")

# ------------------------------------------------------------- API pública

__all__ = [
    "LOCAL_PATTERNS_FILE",
    "IGNORE_MARKER",
    "MAX_FILE_BYTES",
    "Finding",
    "scan_staged",
    "scan_tracked",
]


def scan_staged(repository: repo.Repository) -> list[Finding]:
    """Confere o que está no índice: nomes e linhas adicionadas.

    Levanta RuntimeError se o git falhar e re.error se um padrão local for
    inválido.
    """
    rules = _read_rules(repository)
    findings = [
        f for name in _read_staged_names(repository) for f in _check_name(name)
    ]
    for path, line_no, text in _read_staged_added_lines(repository):
        findings.extend(_check_line(path, line_no, text, rules))
    return findings


def scan_tracked(repository: repo.Repository) -> list[Finding]:
    """Confere todos os arquivos versionados, inteiros, como estão no disco.

    Levanta RuntimeError se o git falhar e re.error se um padrão local for
    inválido.
    """
    rules = _read_rules(repository)
    root = repository.get_root()
    findings: list[Finding] = []
    for name in repository.run_git("ls-files", "-z").split("\0"):
        if not name:
            continue
        findings.extend(_check_name(name))
        for line_no, text in _read_text_lines(root / name):
            findings.extend(_check_line(name, line_no, text, rules))
    return findings


# ---------------------------------------------------------- Funções privadas


def _read_rules(repository: repo.Repository) -> _Rules:
    """Junta as regras fixas com os padrões pessoais do arquivo local."""
    local = repository.get_root() / LOCAL_PATTERNS_FILE
    if not local.is_file():
        return _CONTENT_RULES
    rules = list(_CONTENT_RULES)
    for line in local.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            pattern = re.compile(line, re.IGNORECASE)
            rules.append(("padrão pessoal (local)", pattern))
    return rules


def _read_staged_names(repository: repo.Repository) -> list[str]:
    """Lista os arquivos adicionados ou modificados no índice."""
    output = repository.run_git(
        "diff", "--cached", "--name-only", "--diff-filter=ACMR", "-z"
    )
    return [name for name in output.split("\0") if name]


def _read_staged_added_lines(
    repository: repo.Repository,
) -> Iterator[tuple[str, int, str]]:
    """Percorre o diff do índice e devolve só as linhas adicionadas."""
    diff = repository.run_git(
        "diff",
        "--cached",
        "--no-color",
        "--no-ext-diff",
        "--unified=0",
        "--diff-filter=ACMR",
    )
    path: str | None = None
    in_header = False
    line_no = 0
    for raw in diff.split("\n"):
        if raw.startswith("diff --git "):
            path, in_header = None, True
        elif in_header and raw.startswith("+++ "):
            # Arquivo binário não tem "+++" e fica com path None: pulado.
            target = raw[4:].rstrip("\t")
            path = target[2:] if target.startswith("b/") else None
        elif raw.startswith("@@"):
            in_header = False
            match = _HUNK_HEADER.match(raw)
            line_no = int(match.group(1)) if match else 0
        elif not in_header and path and raw.startswith("+"):
            yield path, line_no, raw[1:]
            line_no += 1


def _read_text_lines(path: Path) -> Iterator[tuple[int, str]]:
    """Lê um arquivo de texto do disco, linha a linha. Binário é pulado."""
    try:
        data = path.read_bytes()
    except OSError:
        return  # Saiu do disco mas segue no índice: o próximo commit resolve
    if len(data) > MAX_FILE_BYTES or b"\0" in data[:8192]:
        return
    text = data.decode("utf-8", "replace")
    yield from enumerate(text.split("\n"), start=1)


def _check_name(name: str) -> list[Finding]:
    """Confere o caminho do arquivo: pasta proibida ou nome de segredo."""
    parts = PurePosixPath(name).parts
    base = parts[-1]
    if _FORBIDDEN_DIRS.intersection(parts[:-1]):
        return [Finding(name, 0, "pasta de nota fiscal", base)]
    if ".example" in base:
        return []
    return [
        Finding(name, 0, rule, base)
        for rule, pattern in _NAME_RULES
        if pattern.search(base)
    ]


def _check_line(
    path: str,
    line_no: int,
    text: str,
    rules: _Rules,
) -> list[Finding]:
    """Confere uma linha contra todas as regras. Uma vez por regra basta."""
    if IGNORE_MARKER in text:
        return []
    is_example = ".example" in PurePosixPath(path).name
    findings: list[Finding] = []
    for rule, pattern in rules:
        if is_example and rule in _PASSWORD_RULES:
            continue
        for match in pattern.finditer(text):
            if rule == "e-mail" and _is_allowed_email(match.group(0)):
                continue
            findings.append(Finding(path, line_no, rule, _mask(match.group(0))))
            break
    return findings


def _is_allowed_email(address: str) -> bool:
    """E-mail genérico (exemplo, noreply, remoto do git) não é dado pessoal."""
    user, _, domain = address.lower().partition("@")
    return user in _EMAIL_ALLOWED_USERS or domain in _EMAIL_ALLOWED_DOMAINS


def _mask(text: str) -> str:
    """Mostra só o começo: o relatório não pode vazar o que ele achou."""
    return f"{text[:4]}…" if len(text) > 6 else "…"
