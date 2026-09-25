# hardware/

A parte elétrica do barco: esquemático, ligações e a lista do que vai no cofre.

> 🚧 **Vazio por enquanto.** O desenho proposto está no
> [esboço, seção 7](../docs/esbocando-a-ideia.md#7-eletrônica-e-firmware), e
> depende das decisões D4 (energia), D5 (controle) e D8 (placa).

## O que vai morar aqui

- O projeto **KiCad** com o **esquemático** — a proposta (D8) é não fabricar
  PCB e montar com módulos numa placa perfurada.
- A pinagem: qual pino da ESP vai em qual coisa. É daqui que o firmware tira os
  números — nunca o contrário.
- A lista de componentes, com link pro datasheet.

## Bancada disponível

| Item | Serve pra |
| --- | --- |
| Fonte de bancada | Alimentar com limite de corrente antes de confiar na bateria |
| Osciloscópio de 1 canal | Ver a queda de tensão quando o motor trava (o brownout da seção 5.3 do esboço) |
| ESP32-C3 SuperMini | O cérebro do barco — já tem uma na mão |
