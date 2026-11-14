// Includes
#include "platform/esp_key_value_store.hpp" // EspKeyValueStore

#include <type_traits> // std::is_same_v

#include "esp_nvs.hpp" // begin_nvs_flash
#include "nvs.h"       // nvs_open, nvs_get_i32, nvs_set_i32, nvs_commit

// O header guarda o handle como uint32_t pra não expor o tipo do ESP-IDF
static_assert(std::is_same_v<nvs_handle_t, uint32_t>);

// Funções públicas
namespace platform {

bool EspKeyValueStore::begin() {
    if (!begin_nvs_flash()) {
        return false;
    }
    nvs_handle_t handle;
    if (nvs_open(_space, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }
    _handle = handle;
    _open   = true;
    return true;
}

bool EspKeyValueStore::read_int(const char *key, int32_t *value) {
    if (!_open) {
        return false;
    }
    return nvs_get_i32(_handle, key, value) == ESP_OK;
}

bool EspKeyValueStore::write_int(const char *key, int32_t value) {
    if (!_open) {
        return false;
    }
    if (nvs_set_i32(_handle, key, value) != ESP_OK) {
        return false;
    }
    return nvs_commit(_handle) == ESP_OK;
}

} // namespace platform
