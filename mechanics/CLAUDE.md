# CLAUDE.md — mechanics/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da mecânica.

## Estado

🚧 Vazio. Nada de modelo até fechar **D2, D3 e D7** do
[esboço](../docs/esbocando-a-ideia.md#11-decisões-em-aberto). O que está abaixo
é proposta pra quando a D7 fechar em OpenSCAD.

## Em aberto que trava peça grande

- **Tamanho da mesa da impressora** e **filamento disponível** — perguntar pro
  dono da impressora. Define se a carapaça sai inteira ou em partes.

## Convenções propostas

- **Paramétrico sempre.** Toda medida é variável nomeada no topo do arquivo, em
  milímetros. Nada de número mágico no meio da geometria.
- **Identificador em inglês, comentário em português** — igual ao firmware.
  Arquivo de modelo em `snake_case.scad`.
- **Uma peça por arquivo.** Medida compartilhada (diâmetro do eixo do motor,
  folga de encaixe) mora num arquivo comum incluído pelos outros.
- **Exportação de trabalho vai pra `out/`**, que é ignorada. A versão que foi de
  fato impressa vai pra `impressos/`, versionada, com a data no nome — é o que
  prova pro juiz quanto filamento foi.
- **Pensado pra imprimir:** base plana, pouco ou nenhum suporte, parede
  generosa. PLA não é estanque de fábrica — o que flutua é o isopor, não a
  peça.

## Não invente

Dimensão de motor, de eixo, de pote e de célula vem de medição com paquímetro
ou de datasheet. Sem medida ainda, a variável nasce com `// a medir`.
