/*
 * Entrada analógica calibrada do ESP32-C3 — o ADC1, a 12 dB (lê 0–2500 mV,
 * datasheet v2.5, p. 57). Implementa AnalogInput.
 */
#pragma once

// Includes
#include <cstdint> // int32_t

#include "platform/analog_input.hpp" // AnalogInput

// Classes
namespace platform {

class EspAnalogInput final : public AnalogInput {
  public:
    // Guarde o pino (tem que ser do ADC1: GPIO0 a GPIO4) — sem mexer no
    // hardware
    constexpr explicit EspAnalogInput(int pin) : _pin(pin) {
    }

    // Ligue o ADC e a calibração; false se o pino não servir
    bool begin();

    // Leia a tensão no pino, em milivolts; -1 se falhar
    int32_t read_mv() override;

  private:
    int _pin;
    int _channel       = -1;
    void *_unit        = nullptr;
    void *_calibration = nullptr;
};

} // namespace platform
