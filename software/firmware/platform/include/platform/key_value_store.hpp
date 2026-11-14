/**
 * @file
 * Abstração: memória que sobrevive ao desligar — guarda número por nome. A
 * camada de cima depende disto; quem implementa na placa é EspKeyValueStore.
 */
#pragma once

// Includes
#include <cstdint> // int32_t

// Classes
namespace platform {

/// Números guardados por nome, que sobrevivem ao desligar
class KeyValueStore {
  public:
    /// Leia o número guardado com esse nome; false se não tem (value fica
    /// como estava)
    virtual bool read_int(const char *key, int32_t *value) = 0;

    /// Guarde o número com esse nome; false se não deu
    virtual bool write_int(const char *key, int32_t value) = 0;

  protected:
    ~KeyValueStore() = default;
};

} // namespace platform
