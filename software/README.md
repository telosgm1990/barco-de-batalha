# software/

O firmware do barco (ESP32-C3) e a página de controle que ele serve pro
celular.

> 🚧 **Só o esqueleto por enquanto:** compila, grava e diz "DonaTelos acordou"
> na serial. O desenho está no
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

O Docker Desktop precisa estar aberto. Todos os comandos rodam da raiz do
repositório, no PowerShell.

**Compilar:**

```powershell
docker run --rm -v "${PWD}\software:/project" -w /project espressif/idf:v6.1 idf.py build
```

**Gravar** (troque `COM5` pela porta que a placa ganhou ao plugar):

```powershell
cd software\build; ..\..\scripts\.venv\Scripts\esptool --chip esp32c3 -p COM5 -b 460800 write-flash "@flash_args"; cd ..\..
```

**Ler a serial** (sai com `Ctrl+]`):

```powershell
scripts\.venv\Scripts\python -m serial.tools.miniterm COM5 115200
```

**Formatar:**

```powershell
scripts\.venv\Scripts\clang-format --style=file:software/.clang-format -i software/main/main.cpp
```
