// Includes
#include "components/battery.hpp" // Battery

// Constantes/variáveis estáticas

// Amostras por leitura: o datasheet sugere média pra melhorar a precisão
static constexpr int SAMPLES = 8;

// Funções públicas
namespace components {

bool Battery::init(int pin, float divider_ratio) {
    _ratio = divider_ratio;
    return _input.init(pin);
}

float Battery::read_volts() {
    int32_t total = 0;
    for (int i = 0; i < SAMPLES; i++) {
        int32_t millivolts = _input.read_mv();
        if (millivolts < 0) {
            return -1.0f;
        }
        total += millivolts;
    }
    return total / static_cast<float>(SAMPLES) / 1000.0f * _ratio;
}

} // namespace components
