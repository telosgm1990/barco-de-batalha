# mechanics/

O corpo da tartaruga: casco, carapaça, rodas-nadadeira, suportes — tudo que vai
pra impressora 3D.

> 🚧 **Modelos paramétricos em OpenSCAD**, com as medidas que ainda não foram
> conferidas marcadas como *a medir* ou *chute*. O casco tem dois caminhos
> ([plano do casco](../docs/plano-do-casco.md)): **A**, pote e isopor — o
> escolhido por ora — e **B**, tudo impresso. Os dois estão projetados aqui.

## Caminho B — tudo impresso

![Caminho B montado, na água](renders/printed_assembly.png)

O **plastrão** (a casca de baixo) é uma banheira com bico de cunha na proa e
um flutuador de cada lado, onde o motor encaixa por cima. A **carapaça** é o
domo com gomos em relevo; ela assenta na borda, prende os motores no berço e
deixa as rodas de pá saírem por dois arcos — as nadadeiras. Atrás, uma
torrinha com a chave liga/desliga. Na frente, a cabeça, de bandana roxa.

![Caminho B explodido](renders/printed_exploded.png)

Por dentro: as duas baterias atravessadas no fundo (lastro) e a placa 5×7 em
cima delas, em quatro colunas. Fechado, o ar preso em cima faz a tartaruga
desvirar sozinha.

| Peça | Arquivo | Caixa (mm) | Filamento (*estimativa*) |
| --- | --- | --- | --- |
| Plastrão | [printed/plastron.scad](printed/plastron.scad) | 166 × 165 × 53 | ~92 g |
| Carapaça | [printed/carapace.scad](printed/carapace.scad) | 173 × 174 × 41 | ~72 g |
| Cabeça | [printed/head.scad](printed/head.scad) | 22 × 28 × 29 | ~3 g |
| Roda de pá (×2) | [paddle_wheel.scad](paddle_wheel.scad) | 52 × 59 × 16 | ~7 g cada |
| **Total** | | | **~180 g** |

O filamento é *estimativa* pelo volume do modelo, supondo paredes de 1,2 mm e
15% de preenchimento — o fatiador do dono da impressora dá o número de
verdade. Tudo cabe na mesa de 18 cm, a carapaça com pouca folga.

**Linha d'água estimada: ~22 mm**, com o barco na casa de 330 g (uma
célula). Sobram ~26 mm de borda livre, e a roda molha mais ou menos metade do
raio. Conta feita pelo volume do casco no modelo; a bacia confirma.

**Pontos fracos, com honestidade:**

- **Vedação.** PLA deixa a água passar entre as camadas. Antes do B, o **teste
  do copo** ([plano do casco](../docs/plano-do-casco.md)).
- **Filamento.** ~180 g é bastante pra um orçamento de R$ 100 — depende do
  preço do grama.
- **Fixação da carapaça:** por enquanto é encaixe na aba do plastrão e fita
  isolante na emenda.

## Arquivos

```
common/
  dimensions.scad       medidas compartilhadas, cada uma com a fonte
  shapes.scad           formas 2D (superelipse, furo de eixo do TT)
  reference_parts.scad  motor, bateria e placa — só pra vista do conjunto
paddle_wheel.scad       roda de pá, igual nos dois caminhos
printed/                caminho B
  hull_params.scad      medidas do caminho B
  plastron.scad         plastrão
  carapace.scad         carapaça
  head.scad             cabeça
  assembly.scad         vista do conjunto (não imprime)
renders/                imagens geradas a partir dos modelos
```

**Medidas de fonte:** o motor TT vem do desenho do fabricante (página do
produto 3777 da Adafruit); a célula, a placa e o TP4056, das páginas da loja.
O resto está marcado *a medir* — o paquímetro manda quando as peças chegarem.

## Gerando

Exportar uma peça pra impressora, sem cor nem material:

```bash
openscad.com --backend=manifold -O export-3mf/color-mode=none -o out/plastron.3mf printed/plastron.scad
```

Gerar a imagem do conjunto (`-D explode=70` levanta a carapaça):

```bash
openscad.com --backend=manifold --render -o renders/printed_assembly.png --imgsize=1400,1000 --colorscheme=Tomorrow --camera=0,0,25,60,0,140,520 printed/assembly.scad
```

## Mandando pra impressora

A impressora é uma mini, com **mesa de 18 cm** — peça maior sai em partes. O
dono dela pediu:

- **`.3mf` sem configuração de filamento** (ou `.stl`, se não der). O filamento
  é escolha dele, na hora de fatiar.
- Uma peça por arquivo, com nome que diz o que é.

Depois de impresso, anotar o **peso em gramas** de cada peça — é assim que o
filamento entra no orçamento.
