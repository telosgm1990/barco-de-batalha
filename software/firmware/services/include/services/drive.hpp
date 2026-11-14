/**
 * @file
 * Tração: o joystick vira velocidade dos dois motores, como tanque de guerra.
 *
 * Joystick pra frente, os dois motores pra frente; pro lado, um vai e o outro
 * volta, e a tartaruga gira no lugar. A saída muda aos poucos (rampa) — motor
 * que arranca de uma vez derruba a tensão e reinicia a ESP (o brownout do
 * esboço, seção 5.3).
 *
 * Cada prova pede um jeito de andar (esboço, seção 7): um modo escolhe o
 * perfil — velocidade máxima, rampa e curva do joystick.
 *
 * Motor nenhum é igual ao outro: o ajuste fino (trim) tira um pouco da força
 * de uma roda, pra o barco andar reto com o joystick reto.
 *
 * Conta pura, sem dependência nenhuma: não tem o que inverter, então não tem
 * abstração — o barco usa direto.
 */
#pragma once

// Enums/structs
namespace services {

/// Um modo por prova
enum class drive_mode_e {
    ECONOMY, ///< Autonomia: devagar e sempre
    PRECISE, ///< Manobrabilidade: controle fino
    FULL,    ///< Arrancada e Salto: tudo
    WAR,     ///< Guerra: tudo, com rampa contra o brownout
};

/// Quantos modos existem (o tamanho da tabela de perfis)
constexpr int DRIVE_MODE_COUNT = 4;

/// O jeito de andar de um modo
struct drive_profile_st {
    float max_output;      ///< velocidade máxima, de 0 a 1
    float slew_per_second; ///< rampa: fração da velocidade cheia por segundo
    float expo;            ///< curva: 0 = reta; 1 = bem suave no meio
};

/// Velocidade das duas rodas
struct wheels_st {
    float left;  ///< -1 a 1
    float right; ///< -1 a 1
};

// Classes

/// Tração de tanque: joystick, curva, mistura, ajuste fino e rampa
class Drive {
  public:
    /// Guarde a zona morta do joystick e o perfil de partida
    constexpr Drive(float deadzone, const drive_profile_st &profile)
        : _deadzone(deadzone), _profile(profile) {
    }

    /// Ande a saída em direção ao joystick (x pro lado, y pra frente, -1 a 1)
    void update(float dt_seconds, float x, float y);

    /// Velocidade atual de cada motor
    wheels_st get_output() const;

    /// Troque o perfil; a rampa leva a saída até o limite novo
    void set_profile(const drive_profile_st &profile);

    /// Ajuste fino, de -1 a 1: positivo tira força da roda direita (o barco
    /// puxava pra esquerda), negativo, da esquerda
    void set_trim(float trim);

    /// Pare na hora, sem rampa — é o que o failsafe usa
    void stop();

    /// Misture o joystick nas duas rodas, sem passar de 1 em nenhuma
    static wheels_st mix(float x, float y);

    /// Curva do joystick: mistura a reta com o cubo, guardando o sinal
    static float shape(float value, float expo);

  private:
    float _deadzone;
    drive_profile_st _profile;
    float _trim       = 0.0f;
    wheels_st _output = {0.0f, 0.0f};
};

// Funções

/// Índice do modo na tabela de perfis
constexpr int mode_index(drive_mode_e mode) {
    return static_cast<int>(mode);
}

} // namespace services
