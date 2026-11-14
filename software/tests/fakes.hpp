/*
 * Peças falsas pros testes do PC: implementam as abstrações de cada camada e
 * guardam o que receberam, pra o teste conferir.
 */
#pragma once

// Includes
#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t
#include <string>  // std::string

#include "components/battery.hpp"      // Battery
#include "components/led.hpp"          // Led
#include "components/motor.hpp"        // Motor
#include "platform/access_point.hpp"   // AccessPoint
#include "platform/analog_input.hpp"   // AnalogInput
#include "platform/clock.hpp"          // Clock
#include "platform/digital_output.hpp" // DigitalOutput
#include "platform/logger.hpp"         // Logger
#include "platform/pwm_output.hpp"     // PwmOutput
#include "platform/web_server.hpp"     // WebServer, TextListener
#include "services/remote.hpp"         // Remote

// Classes
namespace fakes {

// ---------------------------------------------------------------- platform/

class Pwm final : public platform::PwmOutput {
  public:
    void write(float duty) override {
        last_duty = duty;
    }

    float last_duty = -1.0f;
};

class Digital final : public platform::DigitalOutput {
  public:
    void write(bool high) override {
        level = high;
    }

    bool level = false;
};

class Analog final : public platform::AnalogInput {
  public:
    int32_t read_mv() override {
        reads++;
        return millivolts;
    }

    int32_t millivolts = 0;
    int reads          = 0;
};

class Clock final : public platform::Clock {
  public:
    uint32_t millis() override {
        return now_ms;
    }

    void sleep_ms(uint32_t ms) override {
        now_ms += ms;
    }

    uint32_t now_ms = 0;
};

class Logger final : public platform::Logger {
  public:
    void write(
        platform::log_level_e level,
        const char *tag,
        const char *text
    ) override {
        (void)level;
        (void)tag;
        last_line = text;
        lines++;
    }

    std::string last_line;
    int lines = 0;
};

class AccessPoint final : public platform::AccessPoint {
  public:
    bool begin() override {
        started = true;
        return works;
    }

    bool works   = true;
    bool started = false;
};

class WebServer final : public platform::WebServer {
  public:
    bool begin(
        const char *page,
        size_t page_length,
        platform::TextListener &listener
    ) override {
        served_page.assign(page, page_length);
        this->listener = &listener;
        return true;
    }

    bool is_client_open(int client) override {
        return client >= 0 && client == open_client;
    }

    bool send_text(int client, const char *text) override {
        last_client = client;
        last_text   = text;
        return true;
    }

    // Finja que a página mandou uma mensagem
    void receive(const char *text, int client) {
        listener->on_text(text, std::string(text).size(), client);
    }

    std::string served_page;
    platform::TextListener *listener = nullptr;
    int open_client                  = -1;
    int last_client                  = -1;
    std::string last_text;
};

// ---------------------------------------------------------------- components/

class Motor final : public components::Motor {
  public:
    void begin() override {
        speed = 0.0f;
    }

    void set_speed(float new_speed) override {
        speed = new_speed;
    }

    float speed = -9.0f;
};

class Led final : public components::Led {
  public:
    void begin() override {
    }

    void update(uint32_t now_ms) override {
        (void)now_ms;
    }

    void set_blink(uint32_t period_ms) override {
        on       = true;
        blink_ms = period_ms;
    }

    void set_off() override {
        on = false;
    }

    bool on           = false;
    uint32_t blink_ms = 0;
};

class Battery final : public components::Battery {
  public:
    float read_volts() override {
        return volts;
    }

    float volts = 3.9f;
};

// ---------------------------------------------------------------- services/

class Remote final : public services::Remote {
  public:
    bool begin() override {
        started = true;
        return true;
    }

    services::command_st get_command() override {
        return command;
    }

    bool is_connected() override {
        return connected;
    }

    void send_telemetry(const char *text) override {
        last_telemetry = text;
        telemetry_count++;
    }

    services::command_st command =
        {0.0f, 0.0f, 0, false, services::drive_mode_e::ECONOMY, 0};
    bool connected = false;
    bool started   = false;
    std::string last_telemetry;
    int telemetry_count = 0;
};

} // namespace fakes
