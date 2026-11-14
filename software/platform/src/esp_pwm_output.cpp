// Includes
#include "platform/esp_pwm_output.hpp" // EspPwmOutput

#include "driver/ledc.h" // ledc_*

// Constantes/variáveis estáticas
static constexpr ledc_mode_t MODE            = LEDC_LOW_SPEED_MODE;
static constexpr ledc_timer_t TIMER          = LEDC_TIMER_0;
static constexpr ledc_timer_bit_t RESOLUTION = LEDC_TIMER_10_BIT;
static constexpr uint32_t DUTY_MAX           = (1u << 10) - 1;

// Funções públicas
namespace platform {

void EspPwmOutput::begin_timer(uint32_t frequency_hz) {
    ledc_timer_config_t timer = {};
    timer.speed_mode          = MODE;
    timer.duty_resolution     = RESOLUTION;
    timer.timer_num           = TIMER;
    timer.freq_hz             = frequency_hz;
    timer.clk_cfg             = LEDC_AUTO_CLK;
    ledc_timer_config(&timer);
}

void EspPwmOutput::begin() {
    if (_pin < 0 || _channel < 0) {
        return;
    }
    ledc_channel_config_t config = {};
    config.gpio_num              = _pin;
    config.speed_mode            = MODE;
    config.channel               = static_cast<ledc_channel_t>(_channel);
    config.timer_sel             = TIMER;
    config.duty                  = 0;
    config.hpoint                = 0;
    _started                     = ledc_channel_config(&config) == ESP_OK;
}

void EspPwmOutput::write(float duty) {
    if (!_started) {
        return;
    }
    // Limite o ciclo e converta pra contagem do timer
    if (duty < 0.0f) {
        duty = 0.0f;
    }
    if (duty > 1.0f) {
        duty = 1.0f;
    }
    uint32_t counts        = static_cast<uint32_t>(duty * DUTY_MAX + 0.5f);
    ledc_channel_t channel = static_cast<ledc_channel_t>(_channel);
    ledc_set_duty(MODE, channel, counts);
    ledc_update_duty(MODE, channel);
}

} // namespace platform
