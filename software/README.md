# software/

O firmware do barco (ESP32-C3) e a página de controle que ele serve pro
celular.

> 🚧 **Vazio por enquanto.** O desenho está no
> [esboço, seção 7](../docs/esbocando-a-ideia.md#7-eletrônica-e-firmware), com
> D5 (celular via Wi-Fi) e D9 (ESP-IDF em Docker) fechadas.

## O que vai morar aqui

- **Firmware** em C++ com **ESP-IDF**, em camadas.
- **A página de controle** (joystick no celular), embutida no firmware.
- **Testes** do que tem regra de verdade — a mistura joystick → motores, o
  failsafe — rodando no PC, sem placa.

## Ambiente (D9)

| Etapa | Como | Instala algo no PC? |
| --- | --- | --- |
| Compilar | ESP-IDF v6.1 na imagem Docker oficial (`espressif/idf:v6.1`) | Não — só o Docker, que já tem |
| Gravar e ler a serial | `esptool` e `pyserial` no `.venv` de `scripts/` | Não — fica no `.venv` do projeto |
| Formatar | `clang-format` do `.venv`, com o [.clang-format](.clang-format) daqui | Não — idem |

Os comandos entram aqui quando o primeiro código nascer.
