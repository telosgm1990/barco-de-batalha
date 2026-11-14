# scripts/

Utilitários de desenvolvimento. Tudo isolado do sistema: Python no `.venv`
daqui, Node no `node_modules` daqui, ferramenta pesada no Docker.

| Script | Alias | O que faz |
| --- | --- | --- |
| [check_all/](check_all/) | `check-all` | ✅ Roda todas as checagens do repositório de uma vez: segredos, testes, lint, formatação, ERC e, no Docker, firmware e Doxygen |
| [check_secrets/](check_secrets/) | `check-secrets` | 🔒 Caçador de segredos — barra credencial, caminho de máquina, e-mail, CPF e nome antes do commit |
| [install-python-venv.ps1](install-python-venv.ps1) | — | Cria o `.venv` e instala os scripts com seus aliases |
| [git-hooks/pre-commit](git-hooks/pre-commit) | — | Roda o caçador sozinho a cada commit |

[common/](common/) é o código compartilhado entre os scripts — por exemplo o
`repo.Repository` (a abstração do repositório) e o `repo.GitRepository` (o de
verdade). A lógica recebe a abstração por parâmetro; quem monta o de verdade é
o `__main__.py` de cada script. Nos testes entra um falso.

## Testes

A partir da pasta `scripts/`:

```bash
.venv/Scripts/python -m unittest discover -s tests -t .
```

## Criando o ambiente

Uma vez por máquina (e de novo quando entrar script novo):

```bash
powershell -ExecutionPolicy Bypass -File scripts/install-python-venv.ps1
```

Cria `scripts/.venv/` e instala os scripts em modo editável — mexeu no código,
já vale, sem reinstalar. Cada script vira um executável em
`scripts/.venv/Scripts/`, com o nome do alias. `-Force` recria do zero.

Junto vêm as ferramentas do projeto (o `dev` do [pyproject.toml](pyproject.toml)),
cada uma com o seu executável no mesmo lugar:

| Ferramenta | Pra quê |
| --- | --- |
| `ruff` | Lint e formatação do Python |
| `esptool` | Gravar o firmware na ESP |
| `pyserial` | Ler a serial do firmware (`python -m serial.tools.miniterm`) |
| `clang-format` | Formatar o C++ do firmware |

## Rodando

Pelo alias:

```bash
scripts/.venv/Scripts/check-secrets
```

Ou como módulo, a partir da pasta `scripts/` — funciona sem `.venv` quando o
script só usa biblioteca padrão, que é o caso do caçador:

```bash
python -m check_secrets
```

## Todas as checagens

```bash
scripts/.venv/Scripts/check-all
```

Roda, das rápidas pras demoradas: o caçador de segredos (`--all`), os testes
e o lint do Python, a formatação do C++, o ERC do esquemático e, no Docker,
os testes do firmware no PC, a compilação e o Doxygen. Com `--quick`, pula as
do Docker (a rodada inteira leva perto de 1,5 minuto; a rápida, segundos).

Ferramenta que não está na máquina **pula** a checagem em vez de falhar —
Docker Desktop fechado, por exemplo. O `kicad-cli` é procurado na pasta
padrão do KiCad no Windows (a versão mais nova), ou na variável `KICAD_CLI`.

## Caçador de segredos

- `check-secrets` confere o que está no índice (o próximo commit);
  `check-secrets --all`, tudo que já está versionado — bom antes de todo `push`.
- **Padrões pessoais** (nomes, apelidos, e-mails) não podem estar no código,
  que é público. Vão em `scripts/secret-patterns.local.txt`, um regex por linha
  (`#` comenta). Ele está no `.gitignore`.
- Falso alarme? Marque a linha com `check-secrets: ignore`.
- **Ativar como hook** (uma vez por clone), pra rodar sozinho em todo commit:

  ```bash
  git config core.hooksPath scripts/git-hooks
  ```

  O hook usa o alias do `.venv` se ele existir, e o `python -m` se não.
