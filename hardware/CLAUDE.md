# CLAUDE.md — hardware/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da elétrica.

## Estado

🚧 **Esquemático da Fase 1 pronto** (14/11/2026), em `kicad/`, com ERC limpo.
A **pinagem** está na tabela do [README.md](README.md) — é a fonte do
firmware e da [ilustração de montagem](../docs/montagem.md).

- **Símbolos próprios**, em `kicad/donatelos.kicad_sym`: os módulos (ESP,
  TP4056, ponte H) são caixas com os pinos que a placa expõe, todos
  `passive` — assim o ERC só cobra pino solto e rede com um ponto só.
- **Ligação por rótulo** (`V+`, `GND`, `IN1`…), não por fio comprido. Pino que
  não vai em nada leva o "x" de não-conectado.
- **Depois de mexer:** `kicad-cli sch erc` (tem que dar 0 violações),
  conferir as redes com `kicad-cli sch export netlist`, e exportar de novo o
  `esquematico.svg`, que é o que o Capitão lê no GitHub.

## Regras

- **Nunca deduza pino.** A pinagem nasce aqui, no esquemático, e o firmware
  copia. Mudou pino? Muda aqui primeiro, depois no firmware, no mesmo commit.
- **Não invente valor elétrico.** Corrente de motor, capacidade de bateria,
  valor de capacitor: vem de datasheet (com a página) ou de medição na bancada.
  Sem fonte, fica marcado como *a medir*.
- **Custo manda.** Todo componente novo entra no orçamento com preço e origem.
  O teto é R$ 100, e a meta é ficar abaixo de R$ 50.
- **KiCad 10.0** (10.0.8 instalado em 13/11/2026), projeto em
  `hardware/kicad/` quando nascer. Arquivo salvo no 10 não abre em versão
  antiga — quem for abrir precisa do 10 ou mais novo. O PC ainda tem o 7.0
  instalado: **não use**.
- **`kicad-cli`** vem com o KiCad, mas não está no PATH. Use pra checar e
  exportar sem abrir a interface: `kicad-cli sch erc` (regras elétricas) antes
  de dizer que o esquemático está pronto, e `kicad-cli sch export pdf` pra
  quem for ler.
- **Datasheet não vai pro git** (peso e direito autoral). Vai o link, na lista
  de componentes. PDF baixado fica em `hardware/datasheets/`, que é ignorada.

## ESP32-C3 SuperMini — cuidados conhecidos

Fonte: ESP32-C3 Series Datasheet v2.5 (página citada) e a página da placa.

- **GPIO2, GPIO8 e GPIO9 são pinos de strapping** (p. 30): definem o modo de
  boot. Nada que puxe nível neles no boot.
- **GPIO18 e GPIO19 são o USB nativo** (documentação do ESP-IDF) — gravação e
  serial. A SuperMini nem expõe esses dois.
- **GPIO20/21 são a UART0**: o chip escreve nela quando liga. Não serve pra
  motor (daria tranco no boot).
- **ADC:** GPIO0–4 são ADC1 (funciona com Wi-Fi); GPIO5 é ADC2. A 12 dB, o ADC
  lê 0–2500 mV (p. 57).
- **LED da placa no GPIO8, botão BOOT no GPIO9** (página da placa).
- **A ESP aceita 3,0–3,6 V** (p. 55). A bateria entra no 5V, pelo regulador da
  placa — modelo e queda de tensão dele: *a conferir* (a página da placa não
  diz). A fonte de bancada responde até onde a ESP aguenta.
