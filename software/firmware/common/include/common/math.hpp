/*
 * Conta pequena que todo mundo usa: limitar, aproximar, zona morta.
 *
 * Só biblioteca padrão — roda igual na placa e nos testes do PC.
 */
#pragma once

// Includes
#include <cmath> // std::fabs

// Funções
namespace common {

// Limite o valor ao intervalo [low, high]
inline float clamp(float value, float low, float high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

// Ande de current até target, sem passar de max_step por chamada
inline float approach(float current, float target, float max_step) {
    float delta = target - current;
    if (delta > max_step) {
        return current + max_step;
    }
    if (delta < -max_step) {
        return current - max_step;
    }
    return target;
}

// Zere o que estiver perto demais de zero
inline float deadzone(float value, float zone) {
    return std::fabs(value) < zone ? 0.0f : value;
}

} // namespace common
