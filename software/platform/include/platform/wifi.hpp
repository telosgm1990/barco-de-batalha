/*
 * Wi-Fi em modo ponto de acesso: o barco cria a rede, o celular entra nela.
 */
#pragma once

// Includes
#include <cstdint> // int8_t

// Enums/structs
namespace platform::wifi {

struct ap_config_st {
    const char *ssid;
    const char *password; // 8 caracteres ou mais; vazio = rede aberta
    int channel;
    // Potência máxima, em quartos de dBm; 0 = padrão da placa
    int8_t max_tx_power_qdbm;
};

// Funções

// Suba a rede do barco
bool start_ap(const ap_config_st &config);

} // namespace platform::wifi
