# Lista de materiais — Fase 1

O que foi comprado pra **Fase 1, "Boia e anda"** — o barco feio que boia, anda
e grava o vídeo — e quanto disso vai pra conta do barco.

> 📦 **Comprado em 14/11/2026**, numa loja online só, com entrega em Ribeirão
> Preto (a loja estima **24/11**). Os preços são os do carrinho —
> **conferir com a nota** quando ela chegar. A nota fiscal em si **não entra
> no git**: tem CPF, nome e endereço; ela vai pro juiz.
>
> **Só conta o que vai no barco** (regulamento, seção 1). Pacote usado em
> parte entra pela fração usada; peça comprada a mais pra reserva de
> desenvolvimento não entra. Frete também não entra.

Legenda: ✅ já tinha · 📦 comprado · ♻️ reaproveitar (reciclado entra a preço
de kg) · 🖨️ imprimir

## 1. O que vai no barco

| # | Item | Comprado | No barco | Na conta | Obs. |
| --- | --- | --- | --- | --- | --- |
| 1 | ✅ ESP32-C3 SuperMini | já tinha | 1 | R$ 11,00 (cotação) | Veio sem pino: leva a barra macho do item 12 |
| 2 | 📦 Driver ponte H dupla (chip TC1508A) | 2 × R$ 4,99 | 1 | R$ 4,99 | Até 7,2 V e 1,8 A por canal (página da loja). O segundo é reserva |
| 3 | 📦 Motor TT 3–6 V, redução 1:48, eixo duplo | 3 × R$ 8,99 | 2 | R$ 17,98 | 200 RPM a 6 V, sem carga (página da loja). Vem sem fio. O terceiro é reserva |
| 4 | 📦 Célula 18650 GoldHill | 3 × R$ 14,99 | 1 (ou 2) | R$ 14,99 (ou R$ 29,98) | A página da loja diz **700–1100 mAh reais** e **sem chip de proteção** — a proteção vem do item 6. Uma ou duas em paralelo: ver o [plano do casco](plano-do-casco.md#4-a-conta) |
| 5 | 📦 Suporte pra 1× 18650, com fio | 2 × R$ 3,99 | 1 (ou 2) | R$ 3,99 (ou R$ 7,98) | Acompanha a decisão do item 4 |
| 6 | 📦 Carregador TP4056 com proteção (micro-USB) | 1 × R$ 4,99 | 1 | R$ 4,99 | Carrega a 1 A e corta a descarga em ~2,5 V (página da loja). **Sem reserva** |
| 7 | 📦 Chave gangorra KCD11, 3 A | pacote de 3, R$ 5,99 | 1 | R$ 2,00 | |
| 8 | 📦 Capacitor eletrolítico 100 µF 16 V | pacote de 10, R$ 3,99 | 2 (*chute*) | R$ 0,80 | O amortecedor contra o brownout; vários em paralelo se o esquemático pedir. O driver já tem dois desses na plaquinha |
| 9 | 📦 Resistor 100 kΩ | pacote de 20, R$ 3,99 | 2 | R$ 0,40 | O divisor que lê a bateria |
| 10 | 📦 Placa fenolite ilhada 5×7 cm | 2 × R$ 3,99 | 1 | R$ 3,99 | A segunda é pra errar à vontade |
| 11 | 📦 Barra de pinos fêmea 1×20 | pacote de 5, R$ 4,99 | ~1 barra (*chute*) | R$ 1,00 | ESP e driver encaixam, não soldam direto |
| 12 | 📦 Barra de pinos macho 1×40 | pacote de 5, R$ 3,99 | ~1 barra (*chute*) | R$ 0,80 | Na SuperMini e no driver |
| 13 | 📦 Borne KF301, 2 vias | pacote de 5, R$ 6,99 | 3 | R$ 4,19 | Bateria e os dois motores, sem solda |
| 14 | 📦 Cabinho flexível 0,13 mm, preto e vermelho | 2 × 5 m, R$ 4,99 cada | ~1 m de cada (*chute*) | R$ 2,00 | |
| 15 | 📦 Fita isolante 5 m | 2 × R$ 3,99 | um pedaço (*chute*) | R$ 0,50 | No lugar do termorretrátil |
| 16 | ♻️ Pote, isopor | — | depende do casco | ~R$ 1 (*chute*) | Ver o [plano do casco](plano-do-casco.md) |
| 17 | 🖨️ Rodas de pá e suportes de motor | — | 2 + 2 | *chute* R$ 1–3 | Desenhadas depois de medir o motor |

**Na conta do barco, só a eletrônica (itens 1–15):** ~**R$ 74** com uma célula,
~**R$ 93** com duas em paralelo — *estimativa*, porque as frações dos pacotes
são chute. Ainda falta o casco: o [plano do casco](plano-do-casco.md#4-a-conta)
faz a conta inteira.

**Total do pedido:** R$ 150,77 (preço do carrinho, sem frete). A diferença pra
conta do barco é reserva de desenvolvimento — vai que alguma coisa queima.

## 2. Ferramenta e consumível

Não vão no barco, então não entram na conta.

| Item | Situação |
| --- | --- |
| Multímetro, osciloscópio, fonte de bancada | ✅ já tem |
| Ferro de solda + estanho | 🛒 o Capitão vai pegar |
| Cola quente (pistola + bastão) | 🛒 vedar o furo dos fios; e é o kit de fair play |
| Abraçadeira de nylon (enforca-gato) | 🛒 prender motor e fio |
| Cabo **micro-USB** | ✅? Pra carregar pelo TP4056 |
| Cabo USB-C **de dados** | ✅? Pra gravar a ESP — cabo só de carga não funciona |

## 3. Quando a caixa chegar

- **Nota fiscal:** guardar fora do git e conferir os preços desta lista.
- **Células:** sem amassado, sem estufado, sem plástico rasgado. Lítio pede
  respeito, ainda mais perto d'água.
- **Duas em paralelo, só com a mesma tensão.** Antes de ligar uma na outra,
  carregar cada uma sozinha no TP4056 e conferir no multímetro. Célula mais
  cheia despeja corrente na outra na hora em que encosta.
- **TP4056:** conferir que veio a versão **com proteção** (dois chips a mais
  na plaquinha, perto dos terminais da bateria).
- **Medir o motor** com paquímetro: o eixo (diâmetro e o "chato" dele), a caixa
  amarela e a distância entre os furos de fixação. É disso que saem a roda de
  pá e o suporte.

## 4. Fica pra depois

- **Peças de reposição do dia (R$ 10):** sai das reservas — um motor
  (R$ 8,99), ou um driver (R$ 4,99) e mais alguma coisa. Escolher na Fase 4,
  depois de ver o que quebra.
- **Segunda célula no barco:** decisão do [plano do casco](plano-do-casco.md#4-a-conta).
- **Acelerômetro (MPU-6050):** só na Fase 3, se o barco andar torto. Custa
  R$ 19,99 — 20% do teto.
- **Controle físico por ESP-NOW** (segunda ESP + joystick): upgrade da D5, se
  sobrar orçamento.
- **Filamento** da carapaça e das rodas: depende do dono da impressora — falta
  saber o **preço do grama**, que é o que entra na conta.
