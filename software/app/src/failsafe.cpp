// Includes
#include "app/failsafe.hpp" // Failsafe

// Funções públicas
namespace app {

void Failsafe::init(uint32_t timeout_ms) {
    _timeout_ms = timeout_ms;
    _tripped    = true;
}

void Failsafe::update(
    uint32_t now_ms,
    uint32_t last_command_ms,
    bool has_command
) {
    // A conta sem sinal sobrevive à volta do relógio de 32 bits
    _tripped = !has_command || (now_ms - last_command_ms) > _timeout_ms;
}

bool Failsafe::is_tripped() const {
    return _tripped;
}

} // namespace app
