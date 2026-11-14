/*
 * Entrada analógica calibrada — o ADC1 do ESP32-C3, a 12 dB (lê 0–2500 mV,
 * datasheet v2.5, p. 57).
 */
#pragma once

// Includes
#include <cstdint> // int32_t

// Classes
namespace platform {

class AnalogInput {
  public:
    // Prepare o pino (tem que ser do ADC1: GPIO0 a GPIO4)
    bool init(int pin);

    // Leia a tensão no pino, em milivolts; -1 se falhar
    int32_t read_mv();

  private:
    void *_unit        = nullptr;
    void *_calibration = nullptr;
    int _channel       = -1;
};

} // namespace platform
