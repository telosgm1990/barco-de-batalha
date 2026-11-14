# CLAUDE.md — software/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é do firmware.

## Estado

🚧 **Firmware da Fase 1 escrito** (14/11/2026): Wi-Fi em ponto de acesso,
página do joystick por WebSocket, tração com rampa, failsafe, leitura da
bateria e LED de status. Compila e passa nos testes do PC; **ainda não rodou
na placa**. Comandos: [README.md](README.md).

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
main/          app_main + bootstrap: a cola com a placa (pinos, objetos estáticos)
app/           o barco (laço principal) e o failsafe
services/      tração (drive), protocolo da página, controle remoto (remote)
components/    motor, bateria, LED
platform/      ESP-IDF por baixo: PWM, ADC, GPIO, Wi-Fi, HTTP, relógio, log
common/        utilitários de conta
```

Regra de dependência: cada camada só inclui as de baixo; `common/` não inclui
ninguém. `platform/` é a única que conhece o ESP-IDF — é isso que deixa testar
`app/` e `services/` no PC. Precisou incluir camada de cima? O desenho está
errado: pare e discuta.

- **Cada camada é um componente do ESP-IDF:** `app`, `services`, `platform` e
  `common` entram pelo `EXTRA_COMPONENT_DIRS` do `CMakeLists.txt`; cada peça de
  `components/` é um componente sozinha. Include com o nome da camada:
  `"services/drive.hpp"`, `"components/motor.hpp"`.
- **Headers de `platform/` não expõem tipo do ESP-IDF** — o `#include` do
  ESP-IDF fica no `.cpp`.
- **Conta pura vai em arquivo próprio**, sem `platform/`: `drive`, `protocol` e
  `failsafe` são compilados pelos testes em `tests/` (g++ da imagem Docker).
  Regra nova com conta? Ganha teste lá.
- **O serviço do Wi-Fi se chama `remote`, não `link`:** `link()` é função do
  POSIX e o nome colide.
- **A página** (`services/page/index.html`) entra no binário pelo
  `EMBED_TXTFILES` do `CMakeLists.txt` de `services/`.

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
- **Nomes:** `snake_case` pra função, método e variável; `PascalCase` pra
  classe; struct simples com sufixo `_st`, enum com `_e`; constante e macro em
  `MAIUSCULO`. **`_` no começo = privado** (membro e função estática). Serviço
  é um `namespace` (`drive::update()`).
- **Include comentado** com o que veio dali: `#include "motor.hpp" // Motor`.
- **Objetos estáticos**, montados no bootstrap. Nada de `new`/`malloc` depois
  da inicialização.
- **Comentário é foto do agora.** Histórico é do `git log`.

## Não invente

Pino vem de `hardware/`. Tempo de failsafe, rampa de aceleração, duty cycle de
cada modo: **medido na água**, não chutado. Sem medição ainda, a constante nasce
com um comentário `// a medir` e entra na lista do esboço.
