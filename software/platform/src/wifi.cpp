// Includes
#include "platform/wifi.hpp" // wifi

#include <cstring> // strlen, strncpy

#include "esp_event.h" // esp_event_loop_create_default
#include "esp_netif.h" // esp_netif_*
#include "esp_wifi.h"  // esp_wifi_*
#include "nvs_flash.h" // nvs_flash_init

// Protótipos de funções estáticas
static bool _init_nvs();

// Funções públicas
namespace platform::wifi {

bool begin_ap(const ap_config_st &config) {
    if (!_init_nvs()) {
        return false;
    }
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&init_config) != ESP_OK) {
        return false;
    }

    wifi_config_t ap = {};
    size_t ssid_len  = strlen(config.ssid);
    if (ssid_len > sizeof(ap.ap.ssid)) {
        ssid_len = sizeof(ap.ap.ssid);
    }
    memcpy(ap.ap.ssid, config.ssid, ssid_len);
    ap.ap.ssid_len = static_cast<uint8_t>(ssid_len);
    strncpy(
        reinterpret_cast<char *>(ap.ap.password),
        config.password,
        sizeof(ap.ap.password) - 1
    );
    ap.ap.channel        = static_cast<uint8_t>(config.channel);
    ap.ap.max_connection = 1; // um piloto só
    ap.ap.authmode =
        strlen(config.password) >= 8 ? WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN;

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap);
    if (esp_wifi_start() != ESP_OK) {
        return false;
    }
    // Baixe a potência se pedido: antena fraca de alguns lotes da SuperMini
    if (config.max_tx_power_qdbm > 0) {
        esp_wifi_set_max_tx_power(config.max_tx_power_qdbm);
    }
    return true;
}

} // namespace platform::wifi

// Funções estáticas
static bool _init_nvs() {
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES ||
        result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        result = nvs_flash_init();
    }
    return result == ESP_OK;
}
