/**
 * @file
 * Vigia da bateria: avisa quando ela está acabando.
 *
 * A tensão afunda quando o motor puxa forte e volta quando ele alivia. Pra
 * uma arrancada não disparar o aviso, ele só liga depois de a bateria ficar
 * baixa por um tempo seguido (debounce), e só desliga quando ela sobe um
 * pouco acima do limite (histerese). Abaixo de uma tensão mínima, não tem
 * bateria — a placa está só no USB, na bancada —, e aí não tem aviso.
 *
 * Conta pura, sem dependência: o barco usa direto.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Enums/structs
namespace app {

/// Os limites da bateria — vários ainda a medir na bancada
struct battery_watch_st {
    float low_volts;        ///< abaixo disso, está acabando
    float hysteresis_volts; ///< pra desligar, tem que subir isso acima do low
    float absent_volts;     ///< abaixo disso, não tem bateria (placa no USB)
    uint32_t confirm_ms;    ///< quanto tempo baixa, seguido, antes de avisar
};

// Classes

/// Vigia da bateria, com debounce e histerese
class BatteryWatch {
  public:
    /// Guarde os limites; nasce sem aviso
    constexpr explicit BatteryWatch(const battery_watch_st &limits)
        : _limits(limits) {
    }

    /// Confira uma leitura nova da bateria, com a hora da leitura
    void update(uint32_t now_ms, float volts);

    /// Diga se a bateria está acabando
    bool is_low() const;

  private:
    battery_watch_st _limits;
    bool _low                = false;
    bool _below              = false; // a última leitura ficou abaixo do low
    uint32_t _below_since_ms = 0;
};

} // namespace app
