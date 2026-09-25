# software/

O firmware do barco (ESP32-C3) e a página de controle que ele serve pro
celular.

> 🚧 **Vazio por enquanto.** A proposta está no
> [esboço, seção 7](../docs/esbocando-a-ideia.md#7-eletrônica-e-firmware), e
> depende das decisões D5 (controle) e D9 (ambiente).

## O que vai morar aqui

- **Firmware** em C++ com **ESP-IDF**, em camadas.
- **A página de controle** (joystick no celular), embutida no firmware.
- **Testes** do que tem regra de verdade — a mistura joystick → motores, o
  failsafe — rodando no PC, sem placa.

## Ambiente proposto (D9)

| Etapa | Como | Instala algo no PC? |
| --- | --- | --- |
| Compilar | ESP-IDF na imagem Docker oficial da Espressif | Não — só o Docker, que já tem |
| Gravar e ler a serial | `esptool` num `.venv` em `scripts/` | Não — fica no `.venv` do projeto |
| Formatar | `clang-format` com o [.clang-format](.clang-format) daqui | A decidir: Docker ou `.venv` |

Os comandos entram aqui quando o primeiro código nascer.
