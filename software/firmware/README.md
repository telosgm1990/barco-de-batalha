# software/firmware/

O firmware do barco (ESP32-C3) e a página de controle que ele serve pro
celular.

> 🚧 **Firmware da Fase 1 pronto pra bancada:** o barco cria uma rede Wi-Fi,
> serve uma página com joystick e um botão por prova, atende uma API REST,
> mistura o joystick nos dois motores com rampa, para tudo se o celular sumir
> (failsafe) e mostra o nível da bateria na página. Compila sem aviso e passa
> nos testes do PC; **ainda não rodou na placa** — os números marcados
> *a medir* saem da bancada.

## Pilotando

1. Liga o barco. O LED da placa **pisca devagar**: esperando o celular.
2. No celular, entra na rede **DonaTelos**. A senha fica em
   `software/firmware/main/secrets.hpp`, só nesta máquina — o git ignora o arquivo.
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

### Andar reto

Motor nenhum é igual ao outro, e o barco puxa pra um lado com o joystick
reto. Nos botões ◀ ▶ da página, cada toque tira 1% da força de uma roda: o
barco puxa pra esquerda, toque no ▶ até ele andar reto. O ajuste fica
guardado na memória da placa e volta quando ela liga — vale pra todos os
modos. O limite é 30% pra cada lado (*chute*): se precisar de mais, o problema
é mecânico.

## API

A página é só um cliente: qualquer coisa na rede do barco conversa com ele
do mesmo jeito. O joystick vai por **WebSocket** em `/ws`, 20 vezes por
segundo; o resto é uma **API REST** em `/api`, com JSON. O contrato
completo, com exemplo de cada resposta, mora em
[services/include/services/protocol.hpp](services/include/services/protocol.hpp).

| Pedido | Pra quê |
| --- | --- |
| `GET /api/status` | Bateria, failsafe, modo e ajuste fino |
| `GET /api/settings` | Modo e ajuste fino em uso |
| `PATCH /api/settings` | Trocar o modo e/ou o ajuste fino |

Com o PC na rede do barco, dá pra testar no PowerShell, sem página:

```powershell
Invoke-RestMethod http://192.168.4.1/api/status
```

```powershell
Invoke-RestMethod -Method Patch -Uri http://192.168.4.1/api/settings -ContentType application/json -Body '{"mode":"war"}'
```

## Camadas

```
main/          bootstrap: cria as peças concretas com os pinos (board.hpp)
               e injeta cada uma onde precisa
app/           Boat (o laço principal) e Failsafe
services/      Remote -> WifiRemote (Wi-Fi + página + WebSocket + API),
               Drive (joystick -> motores), protocol (o que a página e o
               barco dizem); page/ é a página do joystick
components/    Motor -> HBridgeMotor, Led -> DigitalLed,
               Battery -> DividerBattery
platform/      as abstrações da placa e as concretas Esp* (ESP-IDF por baixo):
               saída digital, PWM, ADC, relógio, log, rede, servidor,
               memória que sobrevive ao desligar
common/        conta pequena: limitar, aproximar, zona morta
tests/         testes no PC, sem placa, com peças falsas (fakes.hpp)
```

`A -> B` é "abstração -> quem implementa": cada camada tem as duas, e a de
cima só conhece a abstração da de baixo (regra 12 do
[CLAUDE.md da raiz](../../CLAUDE.md#12-camadas-e-dependências)). É isso que deixa
o barco inteiro rodar nos testes do PC.

Os pinos em `main/board.hpp` são cópia da
[tabela de pinagem](../../hardware/README.md#pinagem).

## Ambiente (D9)

| Etapa | Como | Instala algo no PC? |
| --- | --- | --- |
| Compilar | ESP-IDF v6.1 na imagem Docker oficial (`espressif/idf:v6.1`) | Não — só o Docker, que já tem |
| Testar no PC | O g++ que já vem na mesma imagem | Não |
| Gravar e ler a serial | `esptool` e `pyserial` no `.venv` de `scripts/` | Não — fica no `.venv` do projeto |
| Formatar | `clang-format` do `.venv`, com o [.clang-format](.clang-format) daqui | Não — idem |
| Documentação | Doxygen, instalado na hora dentro da mesma imagem Docker | Não |

O Docker Desktop precisa estar aberto. Todos os comandos rodam da raiz do
repositório, no PowerShell.

**Senha do Wi-Fi** (uma vez por máquina): copie `software/firmware/main/secrets.example.hpp`
pra `software/firmware/main/secrets.hpp` e troque a senha. Sem ele, o firmware compila
com a senha do exemplo, que é pública.

**Compilar:**

```powershell
docker run --rm -v "${PWD}\software\firmware:/project" -w /project espressif/idf:v6.1 idf.py build
```

**Testar no PC:**

```powershell
docker run --rm -v "${PWD}\software\firmware:/project" -w /project/tests espressif/idf:v6.1 bash -c "cmake -S . -B build -G Ninja && cmake --build build && ./build/host_tests"
```

**Gravar** (troque `COM5` pela porta que a placa ganhou ao plugar):

```powershell
cd software\firmware\build; ..\..\..\scripts\.venv\Scripts\esptool --chip esp32c3 -p COM5 -b 460800 write-flash "@flash_args"; cd ..\..\..
```

**Ler a serial** (sai com `Ctrl+]`):

```powershell
scripts\.venv\Scripts\python -m serial.tools.miniterm COM5 115200
```

**Formatar** (todos os `.cpp` e `.hpp`):

```powershell
Get-ChildItem software\firmware -Recurse -Include *.cpp,*.hpp | Where-Object { $_.FullName -notmatch '\\build\\' } | ForEach-Object { scripts\.venv\Scripts\clang-format --style=file:software/firmware/.clang-format -i $_.FullName }
```

**Documentação** (HTML em `software/firmware/build/doxygen/html/index.html`;
os avisos, se tiver, em `build/doxygen/avisos.txt` — o certo é vazio):

```powershell
docker run --rm -v "${PWD}\software\firmware:/project" -w /project espressif/idf:v6.1 bash -c "apt-get update -qq && apt-get install -y -qq doxygen > /dev/null && mkdir -p build/doxygen && doxygen"
```

## A medir na bancada

Os números de comportamento estão em `main/board.hpp`, cada um com a etiqueta
*a medir* ou *chute*. A lista com o que fazer pra medir cada um está no
[esboço, seção 7](../../docs/esbocando-a-ideia.md#7-eletrônica-e-firmware).
