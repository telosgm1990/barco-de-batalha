# CLAUDE.md — mechanics/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da mecânica.

## Estado

🚧 **D2 e D7 fechadas** em 13/11/2026 (ver o
[esboço](../docs/esbocando-a-ideia.md#11-decisões)): casco híbrido, modelado
em OpenSCAD. A **D3 (roda de pá) é provisória** até o teste da bacia — peça de
propulsão definitiva só depois dele; roda de teste pode.

O casco tem dois caminhos em discussão — pote e isopor (A) ou tudo impresso
(B) — no [plano do casco](../docs/plano-do-casco.md). **Casco e carapaça só
depois da escolha.** Rodas de pá e suportes de motor servem pros dois: podem
nascer já, paramétricos, com as medidas do motor marcadas `// a medir`.

## A impressora

- **Mesa de 18 cm** (uma mini). Peça maior que isso sai em partes, com encaixe
  e cola. A altura máxima é *a confirmar*.
- **Filamento:** em aberto — perguntar pro dono da impressora, junto com o
  **preço do grama**: o filamento que vai no barco entra na conta dos R$ 100.
  Peça leve é peça barata.

## OpenSCAD

- **Snapshot 2026.11.12**, portátil, fora do repo (a estável é de 2021). O
  executável de linha de comando é o `openscad.com`; o caminho na máquina não
  vai pro git.
- Sempre com o motor `manifold`, que é o rápido.
- **Exportação pra impressora**, sem cor nem material (o dono da impressora
  escolhe o filamento na hora de fatiar):

  ```bash
  openscad.com --backend=manifold -O export-3mf/color-mode=none -o out/peca.3mf peca.scad
  ```

## Convenções

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
