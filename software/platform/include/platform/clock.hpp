/*
 * Relógio e espera.
 */
#pragma once

// Includes
#include <cstdint> // uint32_t

// Funções
namespace platform::clock {

// Milissegundos desde que a placa ligou
uint32_t millis();

// Durma a tarefa atual por ms milissegundos
void sleep_ms(uint32_t ms);

} // namespace platform::clock
