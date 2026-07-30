#pragma once
#include <Arduino.h>

namespace Config {

constexpr const char* AP_SSID     = "ESP32-LED";
constexpr const char* AP_PASSWORD = "12345678";
constexpr uint16_t    WS_PORT = 80;
constexpr const char* WS_PATH = "/ws";

}
