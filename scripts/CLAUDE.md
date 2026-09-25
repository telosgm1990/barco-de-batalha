# CLAUDE.md — scripts/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é dos scripts.

## Antes de todo commit

```bash
python scripts/check_secrets.py
```

Achou algo? **Não commite.** Tire do arquivo; se for falso alarme de verdade,
marque a linha com `check-secrets: ignore` e diga no resumo do chat. Antes de
todo `push`, rode com `--all`.

Padrão novo que vale pra todo mundo (tipo de token, formato de dado pessoal) vai
em `_CONTENT_RULES`, no script. Padrão que identifica alguém (nome, e-mail
específico) vai em `secret-patterns.local.txt` — que é ignorado pelo git e não
pode ser citado com conteúdo em lugar nenhum do repo.

## Como escrever script aqui

- **Nada no sistema.** Python sem dependência roda com o Python do PC. Com
  dependência: `scripts/pyproject.toml` + `scripts/.venv/`, e roda com
  `scripts/.venv/Scripts/python.exe`. Node: `scripts/package.json` +
  `node_modules/`. Ferramenta pesada (ESP-IDF, OpenSCAD headless): Docker.
- **Caminho sempre relativo à raiz do repo** — descubra a raiz com
  `git rev-parse --show-toplevel` ou a partir do próprio arquivo. Caminho
  absoluto de máquina nunca entra num script.
- **Estrutura de arquivo fixa**, igual ao resto do projeto: docstring com
  `Uso:` → `# Imports` → `# Constantes de configuração` → `# Tipos públicos` →
  `# Constantes e variáveis privadas` → `__all__` + `# API pública` →
  `# Ponto de entrada` → `# Funções privadas`. O `if __name__ == "__main__":`
  fica no fim do arquivo, porque em Python a função precisa existir antes de
  ser chamada. Copie a estrutura do [check_secrets.py](check_secrets.py).
- **Código em inglês, texto pra pessoa em português.** Nome de arquivo de
  código em `snake_case`.
- PowerShell (`.ps1`) só pro que for coisa de Windows. Hook e shell script em
  `sh`, com final de linha LF (o `.gitattributes` garante).
- Script novo entra na tabela do [README.md](README.md).
