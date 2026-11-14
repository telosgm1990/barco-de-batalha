// Includes
#include "platform/log.hpp" // log

#include <cstdarg> // va_list
#include <cstdio>  // vsnprintf

#include "esp_log.h" // ESP_LOGI, ESP_LOGW

// Constantes/variáveis estáticas
static constexpr int LINE_SIZE = 160;

// Funções públicas
namespace platform::log {

void info(const char *tag, const char *format, ...) {
    char line[LINE_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(line, sizeof(line), format, args);
    va_end(args);
    ESP_LOGI(tag, "%s", line);
}

void warn(const char *tag, const char *format, ...) {
    char line[LINE_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(line, sizeof(line), format, args);
    va_end(args);
    ESP_LOGW(tag, "%s", line);
}

} // namespace platform::log
