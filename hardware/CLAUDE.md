# CLAUDE.md — hardware/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da elétrica.

## Estado

🚧 **D4, D5 e D8 fechadas** em 13/11/2026 (ver o
[esboço](../docs/esbocando-a-ideia.md#11-decisões)): uma 18650, celular via
Wi-Fi, módulos em placa perfurada. O esquemático da Fase 1 espera a lista de
materiais ser comprada — a pinagem depende do módulo que vier da loja.

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

Para conferir no datasheet do ESP32-C3 e no esquemático da placa antes de usar:

- **GPIO2, GPIO8 e GPIO9 são pinos de strapping** (definem o modo de boot).
  Evite ligar neles coisa que puxe o nível no momento do boot.
- **GPIO18 e GPIO19 são o USB nativo** — é por eles que grava e sai a serial.
- Na SuperMini, o **LED da placa** e o **botão BOOT** costumam estar em pinos de
  strapping (8 e 9). Confirmar no esquemático da placa.
- A alimentação de 5 V passa por um regulador da própria placa. Tensão máxima
  de entrada: conferir no datasheet do regulador — é o que decide se a bateria
  liga direto ou precisa de conversor.
