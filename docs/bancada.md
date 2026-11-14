# Bancada — a primeira vez que liga

O roteiro pra ligar a eletrônica da Fase 1 na bancada sem queimar nada, antes
de o barco ver água. E, no caminho, medir os números que nasceram *chute* no
firmware: a lista completa está no [esboço, seção 7.1](esbocando-a-ideia.md#71-a-medir-na-bancada).

> 🔌 **Fonte de bancada antes da bateria.** Com limite de corrente, um curto
> vira um número no visor em vez de fumaça. A bateria só entra depois que
> tudo funcionou na fonte.

Cada número medido vai pro `software/firmware/main/board.hpp`, com a
etiqueta trocada de *chute* pra *medido na bancada*, e pra tabela do esboço.

## 0. A ESP sozinha, no USB

Antes de soldar qualquer coisa:

1. Grave o firmware e abra a serial (comandos no
   [README do firmware](../software/firmware/README.md#ambiente-d9)).
2. Na serial tem que aparecer `DonaTelos acordou. Devagar e sempre.` e
   `rede no ar`.
3. No celular, entre na rede **DonaTelos** e abra **http://192.168.4.1**: a
   página abre, e a bateria mostra perto de 0 V (não tem bateria — e por isso
   também não tem aviso de bateria baixa).
4. Do PC, na mesma rede, a API responde:

   ```powershell
   Invoke-RestMethod http://192.168.4.1/api/status
   ```

**Cuidado com USB e bateria juntos.** Não se sabe ainda se a SuperMini tem
diodo entre o USB e o pino 5V (*a conferir*, na placa ou com o multímetro).
Sem diodo, o 5V do USB entraria no V+ do barco. Até conferir: **ou USB, ou
fonte/bateria** — nunca os dois.

## 1. Antes de ligar: multímetro

Tudo soldado, nada ligado:

- **V+ e GND, em continuidade:** não pode apitar fixo. Um bip curto pode ser
  só os capacitores carregando pela ponta do multímetro (*estimativa*); bip
  que não para é curto.
- **Cada sinal, ponta a ponta:** GPIO4–GPIO7 até IN1–IN4, GPIO3 até o meio do
  divisor, GPIO10 e GPIO1 até os resistores dos LEDs.
- **LEDs:** no modo diodo, o multímetro acende o LED e mostra a queda (Vf).
  Anote: é o que diz se os resistores de 330 Ω e 1 kΩ estão bons
  ([hardware](../hardware/README.md#sinalização)).

## 2. Primeira vez: fonte de bancada

1. Fonte em **3,7 V** (a tensão nominal da 18650), ligada no borne da
   bateria, no lugar dela.
2. **Limite de corrente baixo pra começar** — *chute*: 300 mA, sem motor
   girando. Bateu no limite assim que ligou? Desligue e procure o curto.
3. Ligue a chave. Esperado: **vermelho aceso**, **verde piscando devagar**.
4. Anote a **corrente parada**, com o Wi-Fi no ar e ninguém conectado
   (*a medir*): é a base da conta de autonomia.

## 3. Celular e motores

1. Conecte o celular e abra a página: o **verde fica aceso**.
2. **Suba o limite de corrente aos poucos** até os dois motores girarem com o
   joystick todo — anote a corrente com os motores soltos (*a medir*).
3. **Sentido:** joystick pra frente, as duas rodas pra frente. Uma ao
   contrário? Troque os fios dela no borne, ou o `LEFT_MOTOR_REVERSED` /
   `RIGHT_MOTOR_REVERSED` no `board.hpp`.
4. **Failsafe:** com as rodas girando, desligue o Wi-Fi do celular. Os
   motores param em cerca de meio segundo e o **amarelo pisca rápido**. Parou
   devagar demais, ou para à toa com o sinal bom? É o `FAILSAFE_TIMEOUT_MS`.
5. **Andar reto e modos:** os botões ◀ ▶ e Eco/Fino/Tudo/Guerra respondem, e
   o ajuste fino volta igual depois de desligar e ligar.

## 4. Bateria baixa e o piso da ESP

Ainda na fonte, com o motor girando devagar:

1. **Baixe a tensão** até a faixa amarela aparecer na página e o amarelo
   acender fixo. Confira no multímetro, no borne: era pra ser perto de 3,4 V
   (*chute* do `BATTERY_WATCH`). A leitura do divisor e a do multímetro
   batem? Se não, o `BATTERY_DIVIDER_RATIO` precisa de ajuste.
2. **Continue baixando**, devagar, até a ESP reiniciar. Anote a tensão: é o
   piso do barco. O aviso de bateria baixa tem que vir com folga antes dele.
   A ESP aceita de 3,0 a 3,6 V (datasheet do ESP32-C3, p. 55) — o que passa
   pelo regulador da placa até ela é o que esta medida responde.

## 5. Brownout, no osciloscópio

O motor que arranca de uma vez derruba a tensão e pode reiniciar a ESP
(esboço, seção 5.3).

1. Ponta no **3V3 da ESP**, gatilho na descida, um pouco abaixo da tensão
   normal.
2. Modo **Tudo**, segure uma roda com a mão e dê o joystick todo.
3. A tensão afunda e volta? Ótimo. Afunda até reiniciar (o log mostra o
   reinício)? Diminua a rampa do modo (`PROFILE_*` no `board.hpp`) ou ponha
   mais capacitor perto da ponte H.
4. Repita no modo **Guerra**, que é o que vai empurrar de verdade.

## 6. Só então, a bateria

Célula carregada no TP4056 e conferida no multímetro (4,2 V cheia). Mesma
sequência da seção 2, agora sem limite de corrente: atenção redobrada no
primeiro minuto.
