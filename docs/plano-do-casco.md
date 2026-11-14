# Plano do casco — dois caminhos

O corpo da DonaTelos pode sair de dois jeitos: **A**, com pote e isopor, ou
**B**, com tudo impresso. Este documento põe os dois lado a lado pra o Capitão
escolher.

> 🚧 **Caminho A por ora (14/11/2026).** O Capitão escolheu pote e isopor pra
> começar — é a D2, híbrida, com a carapaça impressa por cima. Mas a mecânica
> **projeta os dois caminhos**: a escolha final espera o preço do filamento, o
> tempo de impressão e quando a impressora fica livre (seção 6).
>
> Número com *estimativa* ou *chute* é isso mesmo: vira medida quando o motor,
> o pote e o fatiador do dono da impressora disserem.

## 0. O que não muda

Seja qual for o casco, por dentro é o mesmo barco:

- **Bateria no fundo**, servindo de lastro (esboço, seção 3.1).
- **Placa 5×7 cm** com a ESP, o driver e o carregador, num lugar seco, com a
  **chave** à mão.
- **Dois motores TT**, um de cada lado, com o eixo pra fora e uma **roda de pá
  impressa** em cada (D3). O motor fica **acima da linha d'água** — o TT não é
  à prova d'água; só a pá molha.
- **A ESP acima d'água**: a antena dela não atravessa água.
- **Impresso nos dois caminhos:** as rodas de pá e os suportes dos motores.
  São peças pequenas; cabem com folga na mesa de 18 cm.

E o prazo manda: campeonato estimado pra **05/12/2026**, peças na mão em
**21/11** (ou 28/11, se o correio atrasar). São **duas semanas de construção**
— o cronograma está no [esboço, seção 13](esbocando-a-ideia.md#13-próximo-passo).

## 1. Caminho A — pote e isopor

A jangada: um pote plástico com tampa de rosca, encaixado numa placa de isopor
recortada em forma de casco. É a D2 como foi decidida.

| Peça | De onde vem | Obs. |
| --- | --- | --- |
| Cofre | ♻️ Pote com tampa de rosca | Boca de pelo menos ~9 cm, pra placa 5×7 entrar deitada (a diagonal dela dá 8,6 cm). Fundo largo pras baterias deitadas |
| Corpo | ♻️ Placa de isopor de embalagem | Recortada em forma de casco, com um furo justo pro pote. Pode passar de 18 cm: isopor não depende da impressora |
| Suportes dos motores | 🖨️ Peça pequena | Presos no isopor com abraçadeira |
| Rodas de pá | 🖨️ Peça pequena | No eixo do TT |
| Carapaça (Fase 2) | 🖨️ Domo, em partes se passar de 18 cm, com isopor dentro | É ela que faz desvirar (esboço, seção 3.1). Sem ela, a jangada é estável dos dois lados — virou, fica virada |

**Passo a passo:**

1. **Esta semana, o Capitão:** achar o pote e o isopor, e medir a boca e a
   altura do pote.
2. **Sábado:** medir o motor. Eu ajusto rodas e suportes, e o dono da
   impressora imprime.
3. Recortar o isopor (estilete ou fio quente) e encaixar o pote, com a bateria
   no fundo.
4. Furo na tampa pros fios dos motores, vedado com cola quente.
5. Motores e rodas nos lados, nos suportes impressos.
6. **Teste da bacia:** boia reto? A pá molha o bastante? Entrou água no pote?
7. **Fase 2:** carapaça impressa por cima, com isopor dentro, e o teste de
   desvirar.

**Cuidados:**

- **Tinta spray derrete isopor.** Pra pintar, tinta acrílica ou PVA, de
  artesanato — ou fita.
- **Cola quente muito quente também derrete isopor.** Pouca cola, ou cola no
  plástico e encosta no isopor.
- **Tampa de rosca:** apertar bem e passar fita isolante na emenda antes de ir
  pra água.

**Ganha:**

- **Rápido:** dá pra montar no mesmo fim de semana em que as peças chegam.
- **Quase de graça:** reciclado entra a preço de kg.
- **Não depende da fila da impressora**, a não ser pelas peças pequenas.
- **Insubmersível por natureza:** cada cm³ de isopor segura quase 1 g. Com o
  barco na casa de 200–350 g (*estimativa*), um bloco de 20×10×2 cm (400 cm³)
  já segura tudo mesmo com o pote cheio d'água.
- **Abre e fecha em 2 segundos**, pra ligar e carregar.

**Perde:**

- **Trabalho manual:** recortar e colar.
- **Cara de tartaruga, só com a carapaça.**
- **O isopor amassa** na pancada da guerra.
- **Motor alinhado no olho:** se o barco andar torto, o firmware compensa.

## 2. Caminho B — tudo impresso

A tartaruga de verdade tem duas cascas: o **plastrão**, a de baixo, da barriga,
e a **carapaça**, o domo. O caminho B imprime as duas, e o vão entre elas é o
cofre.

| Peça | Obs. |
| --- | --- |
| Plastrão | Uma banheira que cabe na mesa de 18 cm **numa peça só** — sem emenda no fundo, sem vazamento no fundo. Com berço pras baterias, apoio da placa, suportes dos motores por fora e furo de fio com ressalto pra cola quente |
| Carapaça | Domo que encaixa num ressalto do plastrão. Se passar de 18 cm, sai em duas metades coladas |
| Junta | ♻️ Tira de câmara de pneu de bicicleta velha no encaixe, e fita isolante por fora (já veio na compra) |
| Flutuação | O ar preso entre as cascas, dividido em **compartimentos**: se um vazar, os outros seguram. Isopor nos vãos como seguro — opcional, quase de graça |
| Rodas de pá | Iguais às do caminho A |

**O ponto fraco é a vedação.** PLA não é estanque de fábrica: a água entra
entre as camadas. Pro B funcionar:

- **Teste do copo, antes de tudo.** Imprimir um copinho com a mesma
  configuração do casco, encher d'água e esperar uma hora. Não vazou, o B
  segue. Vazou, o B fica caro e lento.
- **Pedir mais paredes e mais camadas de fundo** que o normal. Os números
  quem sabe é o dono da impressora, pra impressora dele.
- **Se vazar pouco:** verniz ou cola por dentro. É mais trabalho manual e mais
  custo.

**Passo a passo:**

1. **Esta semana:** o teste do copo, que é peça pequena e sai rápido.
2. **Sábado:** medir o motor. Eu desenho plastrão, carapaça e rodas.
3. Imprimir o plastrão primeiro e testar na bacia vazio, com peso no lugar da
   eletrônica.
4. Montar a eletrônica, fechar com junta e fita, e fazer o teste da bacia.
5. Teste de desvirar.

**Ganha:**

- **Cara de tartaruga desde o começo**, o que conta nos votos de criativo e de
  identidade.
- **Casco duro** pra guerra.
- **Encaixe preciso dos motores:** anda mais reto.
- **Desvira por construção:** domo fechado em cima, bateria embaixo.

**Perde:**

- **Depende do teste do copo e da fila da impressora.** O dono dela imprime
  pra outras equipes também, e a fila aperta perto de 05/12.
- **Mais filamento**, e a conta já está apertada (seção 4).
- **Abrir pra carregar** é tirar a fita e a junta.
- **Peça grande errada** são horas de reimpressão.

## 3. Lado a lado

| | A — pote e isopor | B — tudo impresso |
| --- | --- | --- |
| Pronto pra bacia | No fim de semana em que as peças chegam | Depois do teste do copo e do plastrão impresso |
| Depende da impressora | Rodas e suportes (e a carapaça, na Fase 2) | De tudo |
| Custo do casco | O menor: ~R$ 1 de reciclado + rodas | O maior: plastrão + carapaça + rodas |
| Trabalho manual | Recortar isopor e colar | Vedar e montar a junta |
| Afundar | Difícil: o isopor segura mesmo com água dentro | Depende da vedação e dos compartimentos |
| Desvirar | Só com a carapaça (Fase 2) | Por construção |
| Guerra | O isopor amassa | Casco duro |
| Cara de tartaruga | Com a carapaça | Desde o começo |
| Ligar e carregar | Tampa de rosca, 2 s | Fita e junta |

## 4. A conta

Só conta o que vai no barco (regulamento, seção 1). A eletrônica, pelos preços
da compra e pela parte dos pacotes que entra no barco
([lista de materiais](lista-de-materiais.md)):

| Eletrônica no barco | Na conta (*estimativa*) |
| --- | --- |
| Com **uma** célula | ~R$ 74 |
| Com **duas** em paralelo | ~R$ 93 |

O casco, em *chute* — o fatiador dá os gramas, e falta saber o **preço do
grama** do filamento:

| Casco | Na conta (*chute*) |
| --- | --- |
| Pote e isopor | ~R$ 1 |
| Rodas e suportes | R$ 1–3 |
| Carapaça | R$ 10–25 (o chute do esboço) |
| Plastrão (só no B) | Mais filamento que a carapaça — o maior gasto do B |

Somando:

| | Uma célula | Duas em paralelo |
| --- | --- | --- |
| **A**, só a Fase 1 (sem carapaça) | ~R$ 76–78 | ~R$ 95–97 |
| **A** com a carapaça | ~R$ 86–103 | ~R$ 105–122 ❌ |
| **B** | Acima do A com carapaça — perto ou além do teto | ❌ |

O recado da conta:

- **Duas células no barco só cabem sem carapaça impressa** — ou com células
  quase de graça, tiradas de bateria velha de notebook (pergunta 4 pra
  organização).
- **Com uma célula, o A cabe**, mas a carapaça tem que ser leve: fina, com o
  isopor de dentro fazendo a flutuação.
- **O B é o mais arriscado na conta.**
- **Alavancas, se apertar:** soldar o motor direto e tirar os bornes
  (−R$ 4,19); carapaça mais fina; célula de notebook.

## 5. Recomendação

**Caminho A agora, com a carapaça impressa na Fase 2** — a D2 como está. Por
três motivos:

1. **Prazo.** São duas semanas de construção, e o A boia no mesmo fim de semana
   em que as peças chegam. Isso também segura o vídeo da entrega antecipada.
2. **Conta.** O B gasta mais filamento num orçamento que já está apertado.
3. **Fila.** O A não depende da impressora no fim de novembro, quando todo
   mundo vai querer imprimir.

E **uma célula no barco** (as outras duas GoldHill ficam de reserva), a não
ser que apareça bateria velha de notebook.

O B não morre: o **teste do copo** custa quase nada e pode rodar esta semana.
Se passar e a fila deixar, o plastrão impresso vira upgrade pra final — mas só
depois de o A ter boiado e andado.

## 6. O que falta saber

- **Do Capitão:** o pote (boca e altura), o isopor e, no sábado, as medidas do
  motor.
- **Do dono da impressora** (o Capitão vai perguntar): o filamento (material e
  cor), a altura máxima, o **preço do grama** (é o que entra na conta), **quanto
  tempo** leva pra imprimir cada peça e **quando** a impressora fica livre até
  05/12.
