// Includes
#include "platform/esp_logger.hpp" // EspLogger

#include "esp_log.h" // ESP_LOGI, ESP_LOGW

// Funções públicas
namespace platform {

void EspLogger::write(log_level_e level, const char *tag, const char *text) {
    if (level == log_level_e::WARN) {
        ESP_LOGW(tag, "%s", text);
    }
    else {
        ESP_LOGI(tag, "%s", text);
    }
}

} // namespace platform
