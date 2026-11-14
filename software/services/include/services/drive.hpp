/*
 * Tração: o joystick vira velocidade dos dois motores, como tanque de guerra.
 *
 * Joystick pra frente, os dois motores pra frente; pro lado, um vai e o outro
 * volta, e a tartaruga gira no lugar. A saída muda aos poucos (rampa) — motor
 * que arranca de uma vez derruba a tensão e reinicia a ESP (o brownout do
 * esboço, seção 5.3).
 *
 * Conta pura, sem dependência nenhuma: não tem o que inverter, então não tem
 * abstração — o barco usa direto.
 */
#pragma once

// Enums/structs
namespace services {

struct wheels_st {
    float left;  // -1 a 1
    float right; // -1 a 1
};

// Classes

class Drive {
  public:
    // Guarde a rampa (fração da velocidade cheia por segundo) e a zona morta
    constexpr Drive(float slew_per_second, float deadzone)
        : _slew_per_second(slew_per_second), _deadzone(deadzone) {
    }

    // Ande a saída em direção ao joystick (x pro lado, y pra frente, -1 a 1)
    void update(float dt_seconds, float x, float y);

    // Velocidade atual de cada motor
    wheels_st get_output() const;

    // Pare na hora, sem rampa — é o que o failsafe usa
    void stop();

    // Misture o joystick nas duas rodas, sem passar de 1 em nenhuma
    static wheels_st mix(float x, float y);

  private:
    float _slew_per_second;
    float _deadzone;
    wheels_st _output = {0.0f, 0.0f};
};

} // namespace services
