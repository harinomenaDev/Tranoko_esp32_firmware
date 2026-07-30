#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class LittleFSManager {
public:
    static LittleFSManager& getInstance();
    bool begin(bool formatOnFail = true);
    bool readJson(const String& path, JsonDocument& doc);
    bool writeJson(const String& path, JsonDocument& doc);

private:
    LittleFSManager();
    ~LittleFSManager() = default;
    LittleFSManager(const LittleFSManager&) = delete;
    LittleFSManager& operator=(const LittleFSManager&) = delete;

    bool _mounted = false;
};

#define FSManager LittleFSManager::getInstance()