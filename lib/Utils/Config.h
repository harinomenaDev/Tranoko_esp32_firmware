#pragma once
#include <Arduino.h>

namespace Config {

constexpr const char* AP_SSID     = "ESP32-LED";
constexpr const char* AP_PASSWORD = "12345678";
constexpr uint16_t    WS_PORT = 80;
constexpr const char* WS_PATH = "/ws";


constexpr UBaseType_t deviceQueueLengh    = 16;


constexpr uint32_t    DEVICE_MANAGER_TASK_STACK    = 4096;
constexpr UBaseType_t DEVICE_MANAGER_TASK_PRIORITY = 2;

constexpr BaseType_t CORE_NETWORK = 0;
constexpr BaseType_t CORE_APP     = 1;

constexpr size_t MAX_DEVICES = 10;
}
