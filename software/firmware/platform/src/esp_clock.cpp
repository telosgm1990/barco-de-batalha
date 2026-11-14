// Includes
#include "platform/esp_clock.hpp" // EspClock

#include "esp_timer.h"         // esp_timer_get_time
#include "freertos/FreeRTOS.h" // pdMS_TO_TICKS
#include "freertos/task.h"     // vTaskDelay

// Funções públicas
namespace platform {

uint32_t EspClock::millis() {
    return static_cast<uint32_t>(esp_timer_get_time() / 1000);
}

void EspClock::sleep_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

} // namespace platform
