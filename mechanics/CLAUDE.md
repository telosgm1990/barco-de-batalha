# CLAUDE.md — mechanics/

Regras gerais em [../CLAUDE.md](../CLAUDE.md). Aqui só o que é da mecânica.

## Estado

🚧 **D2 e D7 fechadas** em 13/11/2026 (ver o
[esboço](../docs/esbocando-a-ideia.md#11-decisões)): casco híbrido, modelado
em OpenSCAD. A **D3 (roda de pá) é provisória** até o teste da bacia — peça de
propulsão definitiva só depois dele; roda de teste pode.

O casco tem dois caminhos — pote e isopor (A) ou tudo impresso (B) — no
[plano do casco](../docs/plano-do-casco.md). **Por ora é o A** (14/11/2026),
mas o Capitão quer **os dois projetados**: a escolha final depende do preço do
filamento, do tempo de impressão e da fila da impressora. Rodas de pá e
suportes de motor servem pros dois; tudo paramétrico, com as medidas do motor
e do pote marcadas `// a medir` até chegarem.

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

- **Paramétrico sempre**, em milímetros. Nada de número mágico no meio da
  geometria.
- **O módulo recebe as medidas por parâmetro** (regra 12 do CLAUDE.md da
  raiz), em vez de ler variável global:
  - medida **da própria peça** (quantas pás, espessura da parede) é parâmetro
    com o **valor padrão na assinatura** — o conjunto não precisa saber dela;
  - medida **compartilhada** (motor, eixo, isopor, folga) vem **sem padrão**:
    quem chama passa — o topo do arquivo da peça ou o conjunto;
  - conta derivada que mais de um arquivo usa vira **função** no arquivo da
    peça (`mount_x_min` em `pot/motor_mount.scad`).
  - Já migraram as peças da Fase 1: roda de pá, suporte do motor e molde do
    isopor. O caminho B (`printed/`), as carapaças e o `reference_parts`
    ainda leem global — migram quando forem mexidos.
- **Comentário em cima de cada módulo e função** (regra 13): o que desenha e
  uma linha por parâmetro, com a unidade. Igual ao `common/shapes.scad`.
- **Mudou o código sem querer mudar a forma?** Exporte em STL antes e depois e
  compare: peça sozinha sai idêntica byte a byte; conjunto não (a ordem dos
  triângulos varia), então compare volume e tamanho.
- **Identificador em inglês, comentário em português** — igual ao firmware.
  Arquivo de modelo em `snake_case.scad`.
- **Uma peça por arquivo.** Medida compartilhada (diâmetro do eixo do motor,
  folga de encaixe) mora num arquivo comum incluído pelos outros:
  `common/dimensions.scad` pro barco todo, `printed/hull_params.scad` só pro
  caminho B. A exceção é `common/reference_parts.scad`: modelos do que não se
  imprime (motor, bateria, placa), só pra vista do conjunto.
- **Pastas:** `common/` (medidas e formas), `printed/` (caminho B), `pot/`
  (caminho A), peça dos dois caminhos na raiz. Medida em `include`, módulo em
  `use` — cada `.scad` de peça chama o próprio módulo no topo, e o `use` ignora
  essa chamada.
- **`renders/` é gerada a partir dos modelos**, mas fica versionada: é onde o
  Capitão olha. Mudou o modelo, gere de novo (comandos no README).
- **Exportação de trabalho vai pra `out/`**, que é ignorada. A versão que foi de
  fato impressa vai pra `impressos/`, versionada, com a data no nome — é o que
  prova pro juiz quanto filamento foi.
- **Pensado pra imprimir:** base plana, pouco ou nenhum suporte, parede
  generosa. PLA não é estanque de fábrica — o que flutua é o isopor, não a
  peça.

## Não invente

Dimensão de motor, de eixo, de pote e de célula vem de medição com paquímetro
ou de datasheet. Sem medida ainda, a variável nasce com `// a medir`.
