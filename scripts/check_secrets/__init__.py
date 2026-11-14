"""Caçador de segredos: barra segredo antes de virar história pública no git.

Credencial, caminho de máquina e dado pessoal não entram no repositório.

Confere as linhas **adicionadas** no índice — o que vai entrar no próximo
commit — ou, com `--all`, todos os arquivos versionados. Também barra arquivo
com nome de segredo (`.env`, chave SSH, `secrets.*`) e as pastas de nota
fiscal.

Padrão pessoal (nome, apelido, e-mail específico) não pode morar no código, que
é público. Ele vai em `scripts/secret-patterns.local.txt`: um regex por linha,
`#` comenta, e o arquivo está no `.gitignore`.

Linha que precisa passar mesmo casando com um padrão leva o marcador
`check-secrets: ignore`. Use com parcimônia.

Módulos:

- `scanner` — as regras e a varredura
- `__main__` — a linha de comando (`check-secrets` ou `python -m check_secrets`)
"""
