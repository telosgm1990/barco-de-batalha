/*
 * A placa e os números de ajuste do barco.
 *
 * Os pinos são cópia da tabela de pinagem de hardware/README.md — mudou lá,
 * muda aqui, no mesmo commit. O que está "a medir" sai da bancada ou da bacia
 * e entra na lista do esboço (seção 7).
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

#include "app/battery_watch.hpp" // battery_watch_st
#include "services/drive.hpp"    // drive_profile_st, drive_mode_e

// Defines
namespace board {

// ---------------------------------------------------------------- Pinos

constexpr int PIN_LED_ALERT     = 1;  ///< GPIO1 -> LED amarelo (alerta)
constexpr int PIN_BATTERY_SENSE = 3;  ///< GPIO3, ADC1
constexpr int PIN_LEFT_IN1      = 4;  ///< GPIO4 -> IN1 da ponte H
constexpr int PIN_LEFT_IN2      = 5;  ///< GPIO5 -> IN2
constexpr int PIN_RIGHT_IN1     = 6;  ///< GPIO6 -> IN3
constexpr int PIN_RIGHT_IN2     = 7;  ///< GPIO7 -> IN4
constexpr int PIN_LED_STATUS    = 10; ///< GPIO10 -> LED verde (status)

/// @name Canais do PWM, um por entrada da ponte H
/// @{
constexpr int PWM_LEFT_IN1  = 0;
constexpr int PWM_LEFT_IN2  = 1;
constexpr int PWM_RIGHT_IN1 = 2;
constexpr int PWM_RIGHT_IN2 = 3;
/// @}

// ---------------------------------------------------------------- Circuito

/// Divisor de 100 kΩ + 100 kΩ (esquemático): a ESP vê metade da bateria
constexpr float BATTERY_DIVIDER_RATIO = 2.0f;
/// Os LEDs vão do pino pro GND, pelo resistor: acendem em nível alto
constexpr bool LED_ACTIVE_LOW       = false;
constexpr bool LEFT_MOTOR_REVERSED  = false; ///< a medir, na bancada
constexpr bool RIGHT_MOTOR_REVERSED = false; ///< a medir, na bancada

// ---------------------------------------------------------------- Ajuste

constexpr uint32_t PWM_FREQUENCY_HZ    = 5000;  ///< chute — ajustar na bancada
constexpr uint32_t LOOP_PERIOD_MS      = 20;    ///< 50 voltas por segundo
constexpr uint32_t FAILSAFE_TIMEOUT_MS = 500;   ///< a medir
constexpr float JOYSTICK_DEADZONE      = 0.05f; ///< chute
constexpr uint32_t STATUS_PERIOD_MS    = 500;   ///< retrato pra API

// ---------------------------------------------------------------- Modos

/// @name Um perfil por prova
/// Velocidade máxima, rampa (fração da velocidade cheia por segundo) e curva
/// do joystick. Tudo chute — afinar na bacia.
/// @{
constexpr services::drive_profile_st PROFILE_ECONOMY = {0.5f, 1.0f, 0.0f};
constexpr services::drive_profile_st PROFILE_PRECISE = {0.6f, 2.0f, 0.6f};
constexpr services::drive_profile_st PROFILE_FULL    = {1.0f, 4.0f, 0.0f};
/// Guerra: tudo, mas com rampa contra o brownout no empurrão — a medir
constexpr services::drive_profile_st PROFILE_WAR = {1.0f, 2.0f, 0.2f};
/// @}

/// Liga no econômico: o mais manso, pra primeira vez na bancada
constexpr services::drive_mode_e INITIAL_MODE = services::drive_mode_e::ECONOMY;

/// Ajuste fino pra andar reto, até 30% de uma roda pra cada lado — chute: se
/// precisar de mais, o problema é mecânico (roda torta, eixo pegando)
constexpr float MAX_TRIM = 0.3f;

// ---------------------------------------------------------------- Bateria

/// @name Aviso de bateria baixa (18650 de íon-lítio)
/// A tensão afunda no esforço do motor; o aviso espera ela ficar baixa um
/// tempo seguido. Tudo chute — conferir na bancada com a fonte.
/// @{
constexpr app::battery_watch_st BATTERY_WATCH = {
    3.4f, ///< abaixo disso, avisa
    0.1f, ///< pra desligar o aviso, tem que subir isso acima
    1.0f, ///< abaixo disso, não tem bateria: placa só no USB
    2000, ///< quanto tempo baixa, seguido, antes de avisar
};
/// @}

// ---------------------------------------------------------------- Memória

/// Espaço da memória da placa onde o barco guarda os ajustes (até 15 letras)
constexpr const char *STORE_SPACE = "donatelos";

// ---------------------------------------------------------------- Wi-Fi

constexpr int WIFI_CHANNEL = 1; ///< a medir — troque se tiver interferência
constexpr int8_t WIFI_MAX_TX_POWER_QDBM = 0; ///< 0 = padrão; a medir

} // namespace board
