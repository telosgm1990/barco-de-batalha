// Includes
#include "app/battery_watch.hpp" // BatteryWatch

// Funções públicas
namespace app {

void BatteryWatch::update(uint32_t now_ms, float volts) {
    // Sem bateria (placa no USB): nada pra vigiar
    if (volts < _limits.absent_volts) {
        _low   = false;
        _below = false;
        return;
    }
    // Já avisando: só desliga quando subir além da histerese
    if (_low) {
        if (volts > _limits.low_volts + _limits.hysteresis_volts) {
            _low   = false;
            _below = false;
        }
        return;
    }
    // Acima do limite: zera a contagem
    if (volts >= _limits.low_volts) {
        _below = false;
        return;
    }
    // Abaixo: comece a contar e avise se ficar baixa o tempo todo
    if (!_below) {
        _below          = true;
        _below_since_ms = now_ms;
    }
    if (now_ms - _below_since_ms >= _limits.confirm_ms) {
        _low = true;
    }
}

bool BatteryWatch::is_low() const {
    return _low;
}

} // namespace app
