# Lista de materiais — Fase 1

A lista de compras da **Fase 1, "Boia e anda"**: o barco feio que boia, anda e
grava o vídeo. A tartaruga ainda não tem casco, mas já tem lista de compras.

> 🛒 **Rascunho pra revisão do Capitão.** Compra em loja de eletrônica de São
> Carlos ou no Mercado Livre — o que sair mais rápido e barato. Ele revisa,
> risca o que não der e a gente ajusta.
> Os preços são *chute* (do [esboço, seção 8](esbocando-a-ideia.md#8-orçamento-rascunho))
> até a cotação na loja. **Frete não entra no teto de R$ 100** — mas sai do
> bolso, então compra online vale juntar num pedido só.
>
> **Depois da compra, esta lista vira o orçamento:** a coluna *Pago* recebe o
> preço da nota e a loja. A nota fiscal em si **não entra no git** — tem CPF,
> nome e endereço; ela vai pro juiz.

Legenda: ✅ já tem · 🛒 comprar · ♻️ reaproveitar (reciclado entra a preço de
kg) · 🖨️ imprimir

## 1. Vai no barco

É o que conta no teto de R$ 100.

| # | Item | Qtd | O que pedir na loja | Pra quê | Estimativa | Pago |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | ✅ ESP32-C3 SuperMini | 1 | — | O cérebro: Wi-Fi, página do joystick, failsafe | R$ 11 (cotado) | |
| 2 | 🛒 Módulo driver de motor, 2 canais | 1 | **MX1508** ou **DRV8833**, módulo pronto. **L298N não** — grande, pesado e come tensão | Liga e inverte os dois motores | *chute* R$ 3–8 | |
| 3 | 🛒 Motor DC com caixa de redução | 2 | O **"motor TT" amarelo** de carrinho robô (o de eixo duplo serve) ou **N20 com redução**. Os dois **do mesmo modelo**. **Não serve** o "mini motor DC 3–6 V" sem caixa (o prateado, tamanho 130) — ver seção 3 | As rodas de pá — redução porque pá quer força, não giro | *chute* R$ 10–20 o par | |
| 4 | 🛒 Célula de lítio 18650 | 1 | Nova, de marca conhecida. Fuja de anúncio com capacidade absurda (tipo "9900 mAh") — é golpe | A energia (D4) | *chute* R$ 10–25 com o suporte | |
| 5 | 🛒 Suporte pra 1× 18650, com fio | 1 | Caixinha de pilha pra uma 18650 | Segurar a célula no fundo — ela é o lastro | (no item 4) | |
| 6 | 🛒 Módulo de carga **com proteção** | 1 | **TP4056 com proteção** — a versão com chips extras de proteção (DW01 + 8205), não o TP4056 "pelado" | Carregar a célula pela USB e cortar curto e descarga profunda. Fica no cofre; carrega com a tampa aberta, fora d'água | *a cotar* | |
| 7 | 🛒 Chave liga/desliga pequena | 1 | Chave de alavanca ou deslizante, pra placa | Desligar sem desmontar nada | *a cotar* | |
| 8 | 🛒 Capacitor eletrolítico | 2 | Um valor alto, na casa de centenas de µF, de **16 V** (serve pra uma ou duas células) — *chute*: o esquemático fecha o valor | O amortecedor contra o brownout (esboço, 5.3) | centavos | |
| 9 | 🛒 Resistores | 4 | 100 kΩ — *chute*: o esquemático confirma. Compre uns a mais | O divisor que lê a bateria | centavos | |
| 10 | 🛒 Placa perfurada (ilhada) | 1 | Uma pequena — cabe a ESP, o driver e o módulo de carga | Onde tudo é montado (D8) | *chute* R$ 3–8 junto com 11–14 | |
| 11 | 🛒 Barra de pinos fêmea | 2 | Barra de 40 pinos, de cortar | Encaixar ESP e driver sem soldar direto — trocar peça vira encaixar | (no item 10) | |
| 12 | 🛒 Borne de parafuso, 2 vias | 3 | Borne de placa (tipo KF301) | Ligar bateria e os dois motores sem solda — motor queimado se troca com chave de fenda | (no item 10) | |
| 13 | 🛒 Cabinho flexível fino | 1 m de cada | Vermelho, preto e mais uma cor | As ligações | (no item 10) | |
| 14 | 🛒 Termorretrátil | um pedaço | Fino, pra emenda de fio | Isolar emenda | (no item 10) | |
| 15 | ♻️ Pote plástico com tampa de rosca | 1 | De amendoim, de suplemento… vazio e limpo | O cofre: guarda eletrônica e bateria (D2) | ~R$ 0 | |
| 16 | ♻️ Isopor | o que couber | Sobra de embalagem | Flutuação: oco vira isopor | ~R$ 0 | |
| 17 | 🖨️ Rodas de pá | 2 | Eu desenho no OpenSCAD depois de **medir o eixo do motor** com paquímetro | Propulsão (D3, a testar na bacia) | *chute* R$ 1–3 de filamento | |

**Total estimado: a somar depois da cotação.** O esboço chuta de R$ 50 a R$ 100
pro barco inteiro; a Fase 1 é a maior parte disso, porque a carapaça (Fase 2) é
quase só filamento.

## 2. Ferramenta e consumível

Não vão no barco. Se entram no custo, é a pergunta 3 pra organização — até ela
responder, **guarde a nota de tudo**.

| Item | Situação |
| --- | --- |
| Multímetro, osciloscópio, fonte de bancada | ✅ já tem |
| Ferro de solda + estanho | 🛒 o Capitão vai pegar |
| Cola quente (pistola + bastão) | 🛒 vedar o furo dos fios no cofre; e é o kit de fair play |
| Abraçadeira de nylon (enforca-gato) | 🛒 prender motor e fio |
| Cabo USB-C **de dados** | ✅? Pra gravar a ESP — cabo só de carga não funciona |

## 3. O que conferir na loja

- **Nota fiscal:** o juiz pede. No Mercado Livre, prefira loja oficial ou
  vendedor que emite nota — e guarde a nota fora do git.
- **Motor:** pergunte a **tensão nominal** e anote — ela decide se uma 18650
  dá conta ou se a D4 vai pra duas em série. Os dois motores do **mesmo modelo
  e lote**: motor diferente faz o barco andar torto.
- **Motor sem caixa de redução não serve pra roda de pá.** O "mini motor DC
  3–6 V" prateado (tamanho 130) gira rápido e com pouca força; numa roda de pá
  ele patina na água ou trava. O motor TT é justamente esse motorzinho **com** a
  caixa de redução amarela em volta — é a caixa que troca giro por força. O
  pelado só faz sentido se o teste da bacia mudar a D3 pra hélice — aí ele é a
  escolha clássica. Até lá, não compre: motor parado na gaveta também pesa no
  bônus de economia.
- **Driver:** MX1508 ou DRV8833, tanto faz pro firmware — os dois são
  controlados do mesmo jeito, com duas entradas por motor. Leve o que tiver. Anote qual veio: é dele que sai
  a pinagem.
- **18650:** sem amassado, sem estufado, sem plástico rasgado. Lítio pede
  respeito, ainda mais perto d'água.
- **TP4056:** confira que é a versão **com proteção** (tem mais componentes
  na plaquinha que a versão simples).

## 4. Fica pra depois

- **Peças de reposição (R$ 10):** um motor e um driver reserva — compra depois
  que a Fase 1 mostrar o que quebra.
- **Segunda 18650:** só se a medição pedir (D4).
- **Controle físico por ESP-NOW** (segunda ESP + joystick): upgrade da D5, se
  sobrar orçamento.
- **Filamento da carapaça:** Fase 2, depende do dono da impressora.
