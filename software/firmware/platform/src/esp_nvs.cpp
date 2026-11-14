// Includes
#include "esp_nvs.hpp" // begin_nvs_flash

#include "nvs_flash.h" // nvs_flash_init, nvs_flash_erase

// Constantes/variáveis estáticas
static bool _ready = false;

// Funções públicas
namespace platform {

bool begin_nvs_flash() {
    if (_ready) {
        return true;
    }
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES ||
        result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        result = nvs_flash_init();
    }
    _ready = result == ESP_OK;
    return _ready;
}

} // namespace platform
