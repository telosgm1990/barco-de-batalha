# scripts/

Utilitários de desenvolvimento. Tudo isolado do sistema: dependência Python vai
pro `.venv` daqui, dependência Node pro `node_modules` daqui, ferramenta pesada
pro Docker.

| Script | O que faz |
| --- | --- |
| [check_secrets.py](check_secrets.py) | 🔒 Caçador de segredos — barra credencial, caminho de máquina, e-mail, CPF e nome antes do commit |
| [git-hooks/pre-commit](git-hooks/pre-commit) | Roda o caçador sozinho a cada commit |

## Caçador de segredos

Só biblioteca padrão: roda com qualquer Python ≥ 3.10, sem `.venv`.

```bash
python scripts/check_secrets.py
```

```bash
python scripts/check_secrets.py --all
```

O primeiro confere o que está no índice (o próximo commit); o segundo, tudo que
já está versionado — bom de rodar antes de todo `push`.

**Padrões pessoais** — nomes, apelidos, e-mails — não podem estar no script,
que é público. Crie `scripts/secret-patterns.local.txt`, um regex por linha
(`#` comenta). Ele está no `.gitignore`.

**Ativar como hook** (uma vez por clone), pra ele rodar sozinho em todo
commit:

```bash
git config core.hooksPath scripts/git-hooks
```
