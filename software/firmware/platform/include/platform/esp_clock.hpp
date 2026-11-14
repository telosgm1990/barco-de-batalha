/*
 * Relógio do ESP32-C3 (esp_timer e FreeRTOS) — implementa Clock.
 */
#pragma once

// Includes
#include "platform/clock.hpp" // Clock

// Classes
namespace platform {

class EspClock final : public Clock {
  public:
    // Milissegundos desde que a placa ligou
    uint32_t millis() override;

    // Durma a tarefa atual por ms milissegundos
    void sleep_ms(uint32_t ms) override;
};

} // namespace platform
