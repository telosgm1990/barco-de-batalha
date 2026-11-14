/*
 * Abstração: a rede Wi-Fi do barco (o celular entra nela). A camada de cima
 * depende disto; quem implementa na placa é EspAccessPoint.
 */
#pragma once

// Classes
namespace platform {

class AccessPoint {
  public:
    // Suba a rede; false se não deu
    virtual bool begin() = 0;

  protected:
    ~AccessPoint() = default;
};

} // namespace platform
