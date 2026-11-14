// Includes
#include "platform/esp_analog_input.hpp" // EspAnalogInput

#include "esp_adc/adc_cali.h"        // adc_cali_raw_to_voltage
#include "esp_adc/adc_cali_scheme.h" // adc_cali_create_scheme_curve_fitting
#include "esp_adc/adc_oneshot.h"     // adc_oneshot_*

// Constantes/variáveis estáticas
static constexpr adc_atten_t ATTENUATION = ADC_ATTEN_DB_12;

// Funções públicas
namespace platform {

bool EspAnalogInput::begin() {
    adc_unit_t unit_id;
    adc_channel_t channel;
    if (adc_oneshot_io_to_channel(_pin, &unit_id, &channel) != ESP_OK ||
        unit_id != ADC_UNIT_1) {
        return false;
    }
    _channel = channel;

    adc_oneshot_unit_handle_t unit;
    adc_oneshot_unit_init_cfg_t unit_config = {};
    unit_config.unit_id                     = ADC_UNIT_1;
    if (adc_oneshot_new_unit(&unit_config, &unit) != ESP_OK) {
        return false;
    }
    _unit = unit;

    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.atten                  = ATTENUATION;
    channel_config.bitwidth               = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(unit, channel, &channel_config);

    // Use a calibração de fábrica pra converter em milivolts
    adc_cali_handle_t calibration;
    adc_cali_curve_fitting_config_t cali_config = {};
    cali_config.unit_id                         = ADC_UNIT_1;
    cali_config.chan                            = channel;
    cali_config.atten                           = ATTENUATION;
    cali_config.bitwidth                        = ADC_BITWIDTH_DEFAULT;
    if (adc_cali_create_scheme_curve_fitting(&cali_config, &calibration) ==
        ESP_OK) {
        _calibration = calibration;
    }
    return true;
}

int32_t EspAnalogInput::read_mv() {
    if (_unit == nullptr || _calibration == nullptr) {
        return -1;
    }
    int raw          = 0;
    int millivolts   = 0;
    auto unit        = static_cast<adc_oneshot_unit_handle_t>(_unit);
    auto calibration = static_cast<adc_cali_handle_t>(_calibration);
    if (adc_oneshot_read(unit, static_cast<adc_channel_t>(_channel), &raw) !=
        ESP_OK) {
        return -1;
    }
    if (adc_cali_raw_to_voltage(calibration, raw, &millivolts) != ESP_OK) {
        return -1;
    }
    return millivolts;
}

} // namespace platform
