# CLAUDE.md — scripts/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é dos scripts.

## Antes de todo commit

```bash
scripts/.venv/Scripts/check-secrets
```

Sem `.venv`: `python -m check_secrets` a partir de `scripts/`. Achou algo?
**Não commite.** Tire do arquivo; se for falso alarme de verdade, marque a linha
com `check-secrets: ignore` e diga no resumo do chat. Antes de todo `push`, rode
com `--all`.

Padrão novo que vale pra todo mundo (tipo de token, formato de dado pessoal) vai
em `_CONTENT_RULES`, no [scanner.py](check_secrets/scanner.py). Padrão que
identifica alguém vai em `secret-patterns.local.txt` — ignorado pelo git, e o
conteúdo dele não é citado em lugar nenhum do repo.

## Estrutura de script Python

```
scripts/
  pyproject.toml          dependências + um alias por script em [project.scripts]
  install-python-venv.ps1 cria o .venv e instala tudo em modo editável
  common/                 código compartilhado entre os scripts
  <nome_do_script>/
    __init__.py           docstring do pacote: o que o script faz, módulos
    __main__.py           linha de comando: main() + o disparo no fim
    <modulo>.py           a lógica, quando não couber no __main__
```

- **Todo script é um pacote**, rodável com `python -m <nome>` a partir de
  `scripts/`. Nome do pacote em `snake_case`; nome do alias em `kebab-case`.
- **Script novo entra em três lugares:** o pacote, o `[project.scripts]` (e o
  `include` do `[tool.setuptools.packages.find]`) no
  [pyproject.toml](pyproject.toml), e a tabela do [README.md](README.md).
  Depois, rode o `install-python-venv.ps1` de novo pra criar o alias.
- **`common/`** é pra código que dois scripts usam — não pra "talvez um dia".
  **Nunca caminho absoluto de máquina** num script.
- **Camadas e dependências** (regra da raiz, seção 12): a abstração é um
  `typing.Protocol` e a concreta mora no mesmo módulo (`repo.Repository` e
  `repo.GitRepository`). A lógica recebe a abstração por parâmetro; quem cria
  a concreta é o `__main__.py`.
- **Testes** em `tests/`, com peça falsa no lugar da abstração. Rodar a partir
  de `scripts/`: `python -m unittest discover -s tests -t .`
- **Dependência** entra no `pyproject.toml`, nunca `pip install` solto. Se o
  script só usa biblioteca padrão, diga isso na docstring do `__main__` — é o
  que deixa ele rodar sem `.venv`.

## Convenções de código

- **Estrutura de arquivo fixa**: docstring → `# Imports` →
  `# Constantes de configuração` → `# Tipos públicos` →
  `# Constantes e variáveis privadas` → `__all__` + `# API pública` →
  `# Ponto de entrada` → `# Funções privadas`. O `if __name__ == "__main__":`
  fica no fim do arquivo — em Python, a função precisa existir antes de ser
  chamada. Copie a estrutura do [check_secrets/](check_secrets/).
- **O `__all__` segue a ordem das definições**, não a alfabética (por isso a
  `RUF022` está desligada no `pyproject.toml`).
- **Código em inglês, texto pra pessoa em português.**
- **Docstring em tudo que é público** (regra 13 da raiz), no estilo Google:
  `Args:` e `Returns:` só quando acrescentam. O ruff cobra (regras `D`).
- Lint e formatação, antes de dizer que terminou:

  ```bash
  scripts/.venv/Scripts/ruff check scripts
  ```

  ```bash
  scripts/.venv/Scripts/ruff format scripts
  ```

## PowerShell e shell

- `.ps1` só pro que for coisa de Windows. **Salve em UTF-8 com BOM**: o
  PowerShell 5.1 lê arquivo sem BOM como ANSI e estraga todo acento. O Write
  não põe BOM — regrave o arquivo com BOM depois de criar.
- Hook e `.sh` em `sh`, com final de linha LF (o `.gitattributes` garante) e
  bit de executável (`git update-index --chmod=+x`).
