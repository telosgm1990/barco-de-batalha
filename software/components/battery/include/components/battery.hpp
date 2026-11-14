/*
 * Abstração: a bateria, em volts. A camada de cima depende disto; quem
 * implementa é DividerBattery.
 */
#pragma once

// Classes
namespace components {

class Battery {
  public:
    // Leia a tensão da bateria, em volts; negativo se a leitura falhar
    virtual float read_volts() = 0;

  protected:
    ~Battery() = default;
};

} // namespace components
