/**
 * @file
 * Memória que sobrevive ao desligar, na flash do ESP32-C3 (NVS) — implementa
 * KeyValueStore.
 */
#pragma once

// Includes
#include <cstdint> // int32_t, uint32_t

#include "platform/key_value_store.hpp" // KeyValueStore

// Classes
namespace platform {

/// Um espaço da NVS, a flash de configuração
class EspKeyValueStore final : public KeyValueStore {
  public:
    /// Guarde o nome do espaço (até 15 letras) — sem mexer na flash
    constexpr explicit EspKeyValueStore(const char *space) : _space(space) {
    }

    /// Prepare a flash e abra o espaço; false se não deu
    bool begin();

    /// Leia o número guardado com esse nome; false se não tem
    bool read_int(const char *key, int32_t *value) override;

    /// Guarde o número com esse nome, já gravado na flash; false se não deu
    bool write_int(const char *key, int32_t value) override;

  private:
    const char *_space;
    uint32_t _handle = 0;
    bool _open       = false;
};

} // namespace platform
