/**
 * @file
 * Log na serial do ESP32-C3 (a USB da placa), no formato do ESP-IDF —
 * implementa Logger.
 */
#pragma once

// Includes
#include "platform/logger.hpp" // Logger

// Classes
namespace platform {

/// Log pelo ESP_LOG, na serial da USB
class EspLogger final : public Logger {
  public:
    /// Escreva a linha no log do ESP-IDF
    void write(log_level_e level, const char *tag, const char *text) override;
};

} // namespace platform
