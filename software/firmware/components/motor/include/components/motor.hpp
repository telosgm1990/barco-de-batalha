/**
 * @file
 * Abstração: um motor com velocidade de -1 a 1. A camada de cima (o barco)
 * depende disto; quem implementa é HBridgeMotor.
 */
#pragma once

// Classes
namespace components {

/// Um motor, visto pelo barco
class Motor {
  public:
    /// Comece parado
    virtual void begin() = 0;

    /// Velocidade de -1 (ré cheia) a 1 (frente cheia)
    virtual void set_speed(float speed) = 0;

  protected:
    ~Motor() = default;
};

} // namespace components
