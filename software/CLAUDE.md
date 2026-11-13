# CLAUDE.md — software/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é do firmware.

## Estado

🚧 **D5 e D9 fechadas** em 13/11/2026 (ver o
[esboço](../docs/esbocando-a-ideia.md#11-decisões)): celular via Wi-Fi, e
ESP-IDF v6.1 em Docker. As camadas e as convenções abaixo valem a partir do
primeiro commit de código.

## Ambiente

- **Nada instalado no PC.** Compila no Docker, com a imagem oficial fixada em
  `espressif/idf:v6.1` — trocar de versão é decisão, não detalhe. Grava e lê
  serial com `esptool`/`pyserial`, e formata com `clang-format`, todos no
  `.venv` de `scripts/`.
- **Build é recriável:** `build/`, `sdkconfig` e `managed_components/` ficam
  fora do git. O que se versiona é o `sdkconfig.defaults`.
- **Segredo do firmware** (senha do Wi-Fi do barco etc.) fica em
  `secrets.hpp`, ignorado. No git vai o `secrets.example.hpp` com valor falso.

## Camadas propostas

```
main/          app_main + bootstrap: a cola com a placa (pinos, objetos estáticos)
app/           o barco: modo de prova, failsafe
services/      tração, link (Wi-Fi e página), telemetria
components/    motor, bateria, LED
platform/      ESP-IDF por baixo: PWM, ADC, GPIO, Wi-Fi
common/        tipos, log, utilitários
```

Regra de dependência: cada camada só inclui as de baixo; `common/` não inclui
ninguém. `platform/` é a única que conhece o ESP-IDF — é isso que deixa testar
`app/` e `services/` no PC. Precisou incluir camada de cima? O desenho está
errado: pare e discuta.

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
