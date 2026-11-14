# CLAUDE.md — software/firmware/

Regras gerais em [../../CLAUDE.md](../../CLAUDE.md) e
[../CLAUDE.md](../CLAUDE.md). Aqui só o que é do firmware.

## Estado

🚧 **Firmware da Fase 1 escrito:** Wi-Fi em ponto de acesso, página do
joystick por WebSocket, API REST, tração com rampa, um modo por prova, ajuste
fino guardado na placa, failsafe, leitura da bateria e LED de status. Compila
e passa nos testes do PC; **ainda não rodou na placa**. Comandos:
[README.md](README.md).

**Antes de dizer que terminou:** compile, rode os testes do PC e passe o
`clang-format` em tudo — os três comandos estão no README.

## Ambiente

- **Nada instalado no PC.** Compila no Docker, com a imagem oficial fixada em
  `espressif/idf:v6.1` — trocar de versão é decisão, não detalhe. Grava e lê
  serial com `esptool`/`pyserial`, e formata com `clang-format`, todos no
  `.venv` de `scripts/`.
- **Build é recriável:** `build/`, `sdkconfig` e `managed_components/` ficam
  fora do git. O que se versiona é o `sdkconfig.defaults`.
- **Segredo do firmware** (senha do Wi-Fi do barco etc.) fica em
  `secrets.hpp`, ignorado. No git vai o `secrets.example.hpp` com valor falso.

## Camadas

```
main/          bootstrap: o único que conhece as concretas — cria e injeta tudo
app/           Boat (o laço principal) e Failsafe
services/      Remote → WifiRemote; Drive (tração); protocol (WebSocket e
               API REST)
components/    Motor → HBridgeMotor, Led → DigitalLed, Battery → DividerBattery
platform/      DigitalOutput → EspDigitalOutput, PwmOutput → EspPwmOutput,
               AnalogInput → EspAnalogInput, Clock → EspClock,
               Logger → EspLogger, AccessPoint → EspAccessPoint,
               WebServer → EspWebServer, KeyValueStore → EspKeyValueStore
common/        o que mais de uma camada usa e não cabe em nenhuma (math.hpp)
```

`A → B` é "abstração → quem implementa". A regra é a do
[CLAUDE.md da raiz](../../CLAUDE.md#12-camadas-e-dependências), aplicada assim:

- **Cada camada define as próprias abstrações e as implementa ela mesma.** A
  abstração tem o próprio header (`platform/digital_output.hpp`); a concreta,
  outro (`platform/esp_digital_output.hpp`). A camada de cima só inclui o da
  abstração.
- **A camada de cima recebe a abstração da de baixo por referência, no
  construtor** — nunca cria nem procura a concreta. Quem monta é o `main/`.
- **Abstração em C++:** classe com funções `virtual ... = 0` e destrutor
  `protected` não virtual (ninguém apaga uma peça pela abstração; assim ela
  continua `constinit`). Concreta: `final`, com `override`.
- **Conta pura sem dependência** (`Drive`, `Failsafe`, `protocol`) não tem o
  que inverter: é usada direto.
- **Cada camada é um componente do ESP-IDF:** `app`, `services`, `platform` e
  `common` entram pelo `EXTRA_COMPONENT_DIRS` do `CMakeLists.txt`; cada peça de
  `components/` é um componente sozinha. Include com o nome da camada:
  `"services/remote.hpp"`, `"components/motor.hpp"`.
- **Só as concretas `Esp*` da `platform/` conhecem o ESP-IDF**, e só no
  `.cpp`. Todo o resto compila no PC: os testes em `tests/` montam cada
  camada com peças falsas (`tests/fakes.hpp`) no lugar das abstrações de baixo
  — até o `Boat` inteiro. Classe nova ganha teste lá.
- **A página** (`services/page/index.html`) entra no binário pelo
  `EMBED_TXTFILES` do `CMakeLists.txt` de `services/`; o `main/` entrega ela
  pro `WifiRemote` (`services/page.hpp`).

## Convenções de código

- **Idioma:** código em inglês; comentário em português, no imperativo, dizendo
  o passo (`// Pare os motores se o link caiu`).
- **Formatação:** [.clang-format](.clang-format) — 80 colunas, 4 espaços, `else`
  em linha nova.
- **Estrutura de arquivo fixa**, marcada por comentário de seção. Use só as que
  existirem, nunca troque a ordem:
  - `.hpp`: `// Includes` → `// Defines` → `// Enums/structs` → `// Classes` →
    `// Funções`
  - `.cpp`: `// Includes` (o próprio header primeiro) →
    `// Constantes/variáveis estáticas` → `// Protótipos de funções estáticas` →
    `// Funções públicas` → `// Ponto de entrada` → `// Funções estáticas`
- **Público antes de privado**, no arquivo e na classe.
- **Ordem das funções:** `init` → `deinit` → `begin` → `update` → obtenção
  (`get`/`read`/`is`/`was`) → execução (`set`/`write` e ações).
- **`init` × `begin`** (estilo do Capitão): **`init` só inicializa variáveis e
  structs** — guarda pino, canal, configuração, zera estado; não mexe em
  hardware. **`begin` dá a partida em processos** — configura o periférico,
  sobe o Wi-Fi, roda a rotina de reset de um CI. No bootstrap: todo mundo faz
  `init` primeiro, depois todo mundo faz `begin`.
- **Em classe, o `init` é o construtor** — `constexpr` sempre que der. As
  peças são objetos estáticos, construídos antes do `app_main`: o construtor
  só guarda valores e nunca encosta em hardware nem no ESP-IDF. No `main/`
  elas são `constinit` — se um construtor deixar de ser `constexpr`, o
  compilador reclama.
- **Nomes:** `snake_case` pra função, método e variável; `PascalCase` pra
  classe; struct simples com sufixo `_st`, enum com `_e`; constante e macro em
  `MAIUSCULO`. **`_` no começo = privado** (membro e função estática). Cada
  camada é um `namespace` (`platform::`, `components::`, `services::`,
  `app::`). Concreta da placa leva `Esp` na frente (`EspDigitalOutput`).
- **Include comentado** com o que veio dali: `#include "motor.hpp" // Motor`.
- **Objetos estáticos**, montados no bootstrap. Nada de `new`/`malloc` depois
  da inicialização.
- **Comentário é foto do agora.** Histórico é do `git log`.

## Não invente

Pino vem de `hardware/`. Tempo de failsafe, rampa de aceleração, duty cycle de
cada modo: **medido na água**, não chutado. Sem medição ainda, a constante nasce
com um comentário `// a medir` e entra na lista do esboço.
