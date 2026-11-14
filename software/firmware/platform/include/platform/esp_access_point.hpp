/*
 * Ponto de acesso Wi-Fi do ESP32-C3 — implementa AccessPoint.
 */
#pragma once

// Includes
#include <cstdint> // int8_t

#include "platform/access_point.hpp" // AccessPoint

// Enums/structs
namespace platform {

struct ap_config_st {
    const char *ssid;
    const char *password; // 8 caracteres ou mais; vazio = rede aberta
    int channel;
    // Potência máxima, em quartos de dBm; 0 = padrão da placa
    int8_t max_tx_power_qdbm;
};

// Classes

class EspAccessPoint final : public AccessPoint {
  public:
    // Guarde a configuração da rede — sem mexer no rádio
    constexpr explicit EspAccessPoint(const ap_config_st &config)
        : _config(config) {
    }

    // Suba a rede do barco
    bool begin() override;

  private:
    ap_config_st _config;
};

} // namespace platform
