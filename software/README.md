# software/

O firmware do barco (ESP32-C3) e a página de controle que ele serve pro
celular.

> 🚧 **Firmware da Fase 1 pronto pra bancada:** o barco cria uma rede Wi-Fi,
> serve uma página com joystick e um botão por prova, mistura o joystick nos dois motores com rampa,
> para tudo se o celular sumir (failsafe) e manda o nível da bateria pra
> página. Compila sem aviso e passa nos testes do PC; **ainda não rodou na
> placa** — os números marcados *a medir* saem da bancada.

## Pilotando

1. Liga o barco. O LED da placa **pisca devagar**: esperando o celular.
2. No celular, entra na rede **DonaTelos**. A senha fica em
   `software/main/secrets.hpp`, só nesta máquina — o git ignora o arquivo.
3. Abre **http://192.168.4.1** e pilota com o joystick. O LED fica **aceso**.

| LED | Quer dizer |
| --- | --- |
| Pisca devagar | Esperando o celular abrir a página |
| Aceso | No controle |
| Pisca rápido | **Failsafe:** a página parou de mandar comando, motores parados |

A página manda o joystick 20 vezes por segundo, mesmo parada. Se nada chegar
por meio segundo (*a medir*), o barco para os motores — barco sem controle
andando sozinho é pior que barco parado.

### Modos por prova

Os quatro botões em cima do joystick trocam o jeito de andar. O barco liga no
**Eco**, o mais manso, e a página acende o modo que ele confirmou.

| Botão | Pra prova | Jeito |
| --- | --- | --- |
| **Eco** | Autonomia | Metade da força e rampa mansa: devagar e sempre |
| **Fino** | Manobrabilidade | Pouca força e joystick suave no meio, pra contornar boia sem encostar |
| **Tudo** | Arrancada e Salto | Força toda, rampa rápida |
| **Guerra** | Grande Final | Força toda, mas com rampa pra o empurrão não derrubar a placa |

Os números de cada modo (força máxima, rampa e curva do joystick) estão em
`main/board.hpp` — todos *chute* até a bacia.

## Camadas

```
main/          bootstrap: cria as peças concretas com os pinos (board.hpp)
               e injeta cada uma onde precisa
app/           Boat (o laço principal) e Failsafe
services/      Remote -> WifiRemote (Wi-Fi + página + WebSocket), Drive
               (joystick -> motores), protocol; page/ é a página do joystick
components/    Motor -> HBridgeMotor, Led -> DigitalLed,
               Battery -> DividerBattery
platform/      as abstrações da placa e as concretas Esp* (ESP-IDF por baixo):
               saída digital, PWM, ADC, relógio, log, rede, servidor
common/        conta pequena: limitar, aproximar, zona morta
tests/         testes no PC, sem placa, com peças falsas (fakes.hpp)
```

`A -> B` é "abstração -> quem implementa": cada camada tem as duas, e a de
cima só conhece a abstração da de baixo (regra 12 do
[CLAUDE.md da raiz](../CLAUDE.md#12-camadas-e-dependências)). É isso que deixa
o barco inteiro rodar nos testes do PC.

Os pinos em `main/board.hpp` são cópia da
[tabela de pinagem](../hardware/README.md#pinagem).

## Ambiente (D9)

| Etapa | Como | Instala algo no PC? |
| --- | --- | --- |
| Compilar | ESP-IDF v6.1 na imagem Docker oficial (`espressif/idf:v6.1`) | Não — só o Docker, que já tem |
| Testar no PC | O g++ que já vem na mesma imagem | Não |
| Gravar e ler a serial | `esptool` e `pyserial` no `.venv` de `scripts/` | Não — fica no `.venv` do projeto |
| Formatar | `clang-format` do `.venv`, com o [.clang-format](.clang-format) daqui | Não — idem |

O Docker Desktop precisa estar aberto. Todos os comandos rodam da raiz do
repositório, no PowerShell.

**Senha do Wi-Fi** (uma vez por máquina): copie `software/main/secrets.example.hpp`
pra `software/main/secrets.hpp` e troque a senha. Sem ele, o firmware compila
com a senha do exemplo, que é pública.

**Compilar:**

```powershell
docker run --rm -v "${PWD}\software:/project" -w /project espressif/idf:v6.1 idf.py build
```

**Testar no PC:**

```powershell
docker run --rm -v "${PWD}\software:/project" -w /project/tests espressif/idf:v6.1 bash -c "cmake -S . -B build -G Ninja && cmake --build build && ./build/host_tests"
```

**Gravar** (troque `COM5` pela porta que a placa ganhou ao plugar):

```powershell
cd software\build; ..\..\scripts\.venv\Scripts\esptool --chip esp32c3 -p COM5 -b 460800 write-flash "@flash_args"; cd ..\..
```

**Ler a serial** (sai com `Ctrl+]`):

```powershell
scripts\.venv\Scripts\python -m serial.tools.miniterm COM5 115200
```

**Formatar** (todos os `.cpp` e `.hpp`):

```powershell
Get-ChildItem software -Recurse -Include *.cpp,*.hpp | Where-Object { $_.FullName -notmatch '\\build\\' } | ForEach-Object { scripts\.venv\Scripts\clang-format --style=file:software/.clang-format -i $_.FullName }
```

## A medir na bancada

Os números de comportamento estão em `main/board.hpp`, cada um com a etiqueta
*a medir* ou *chute*. A lista com o que fazer pra medir cada um está no
[esboço, seção 7](../docs/esbocando-a-ideia.md#7-eletrônica-e-firmware).
