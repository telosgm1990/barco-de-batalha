# CLAUDE.md — software/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que vale pra todo o
software.

- **Um projeto por pasta**, cada uma com o seu `CLAUDE.md` e `README.md`:
  [firmware/](firmware/CLAUDE.md) é a placa. O app do celular, se sair (D11
  do esboço), nasce em `mobile/` — não crie a pasta antes da decisão.
- **O contrato entre eles é a API do firmware** (WebSocket pro joystick, REST
  pro resto), descrita em `firmware/services/include/services/protocol.hpp`.
  Mudou a API, muda a página no mesmo commit — e o app, quando existir.
- Projeto novo segue a regra de camadas (§12 do CLAUDE.md da raiz) e a de
  documentação (§13), no equivalente da linguagem dele.
