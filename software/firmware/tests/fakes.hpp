/*
 * Peças falsas pros testes do PC: implementam as abstrações de cada camada e
 * guardam o que receberam, pra o teste conferir.
 */
#pragma once

// Includes
#include <cstddef> // size_t
#include <cstdint> // int32_t, uint32_t
#include <map>     // std::map
#include <string>  // std::string

#include "components/battery.hpp"       // Battery
#include "components/led.hpp"           // Led
#include "components/motor.hpp"         // Motor
#include "platform/access_point.hpp"    // AccessPoint
#include "platform/analog_input.hpp"    // AnalogInput
#include "platform/clock.hpp"           // Clock
#include "platform/digital_output.hpp"  // DigitalOutput
#include "platform/key_value_store.hpp" // KeyValueStore
#include "platform/logger.hpp"          // Logger
#include "platform/pwm_output.hpp"      // PwmOutput
#include "platform/web_server.hpp"      // WebServer, TextListener
#include "services/remote.hpp"          // Remote

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
        platform::TextListener &stream,
        platform::RequestListener &api
    ) override {
        served_page.assign(page, page_length);
        this->stream = &stream;
        this->api    = &api;
        return true;
    }

    bool is_client_open(int client) override {
        return client >= 0 && client == open_client;
    }

    // Finja que a página mandou uma mensagem pelo WebSocket
    void receive(const char *text, int client) {
        stream->on_text(text, std::string(text).size(), client);
    }

    // Finja um pedido da API; guarda a resposta e devolve o status HTTP
    int request(
        platform::http_method_e method,
        const char *path,
        const char *body = ""
    ) {
        char out[160] = "";
        int status    = api->on_request(method, path, body, out, sizeof(out));
        last_response = out;
        return status;
    }

    std::string served_page;
    platform::TextListener *stream = nullptr;
    platform::RequestListener *api = nullptr;
    int open_client                = -1;
    std::string last_response;
};

class Store final : public platform::KeyValueStore {
  public:
    bool read_int(const char *key, int32_t *value) override {
        auto found = values.find(key);
        if (found == values.end()) {
            return false;
        }
        *value = found->second;
        return true;
    }

    bool write_int(const char *key, int32_t value) override {
        values[key] = value;
        writes++;
        return true;
    }

    std::map<std::string, int32_t> values;
    int writes = 0;
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

    services::settings_st get_settings() override {
        return settings;
    }

    bool is_connected() override {
        return connected;
    }

    void publish_status(const services::status_st &status) override {
        last_status = status;
        status_count++;
    }

    services::command_st command = {0.0f, 0.0f, 0, false};
    services::settings_st settings =
        {services::drive_mode_e::ECONOMY, 0, 0.0f, 0};
    bool connected                  = false;
    bool started                    = false;
    services::status_st last_status = {};
    int status_count                = 0;
};

} // namespace fakes
