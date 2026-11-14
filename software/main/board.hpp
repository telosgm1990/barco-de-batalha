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

// Defines
namespace board {

// ---------------------------------------------------------------- Pinos

constexpr int PIN_BATTERY_SENSE = 3; // GPIO3, ADC1
constexpr int PIN_LEFT_IN1      = 4; // GPIO4 -> IN1 da ponte H
constexpr int PIN_LEFT_IN2      = 5; // GPIO5 -> IN2
constexpr int PIN_RIGHT_IN1     = 6; // GPIO6 -> IN3
constexpr int PIN_RIGHT_IN2     = 7; // GPIO7 -> IN4
constexpr int PIN_LED           = 8; // GPIO8, LED da placa

// Canais do PWM, um por entrada da ponte H
constexpr int PWM_LEFT_IN1  = 0;
constexpr int PWM_LEFT_IN2  = 1;
constexpr int PWM_RIGHT_IN1 = 2;
constexpr int PWM_RIGHT_IN2 = 3;

// ---------------------------------------------------------------- Circuito

// Divisor de 100 kΩ + 100 kΩ (esquemático): a ESP vê metade da bateria
constexpr float BATTERY_DIVIDER_RATIO = 2.0f;
constexpr bool LED_ACTIVE_LOW = true; // a medir — se o LED inverter, troque
constexpr bool LEFT_MOTOR_REVERSED  = false; // a medir, na bancada
constexpr bool RIGHT_MOTOR_REVERSED = false; // a medir, na bancada

// ---------------------------------------------------------------- Ajuste

constexpr uint32_t PWM_FREQUENCY_HZ    = 5000;  // chute — ajustar na bancada
constexpr uint32_t LOOP_PERIOD_MS      = 20;    // 50 voltas por segundo
constexpr uint32_t FAILSAFE_TIMEOUT_MS = 500;   // a medir
constexpr float DRIVE_SLEW_PER_SECOND  = 3.0f;  // a medir — rampa anti-brownout
constexpr float JOYSTICK_DEADZONE      = 0.05f; // chute
constexpr uint32_t TELEMETRY_PERIOD_MS = 500;

// ---------------------------------------------------------------- Wi-Fi

constexpr int WIFI_CHANNEL = 1; // a medir — troque se tiver interferência
constexpr int8_t WIFI_MAX_TX_POWER_QDBM = 0; // 0 = padrão; a medir

} // namespace board
