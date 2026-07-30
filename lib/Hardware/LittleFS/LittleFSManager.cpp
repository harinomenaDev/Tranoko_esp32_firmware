
#include "LittleFSManager.h"

LittleFSManager& LittleFSManager::getInstance() {
    static LittleFSManager instance;
    return instance;
}

LittleFSManager::LittleFSManager() : _mounted(false) {}

bool LittleFSManager::begin(bool formatOnFail) {
    if (_mounted) {
        return true;
    }

    _mounted = LittleFS.begin(formatOnFail);

    if (!_mounted) {
        Serial.println("[LittleFSManager] Echec du montage LittleFS");
        return false;
    }

    Serial.println("[LittleFSManager] LittleFS monte avec succes");
    return true;
}

bool LittleFSManager::readJson(const String& path, JsonDocument& doc) {
    if (!_mounted) {
        Serial.println("[LittleFSManager] readJson: FS non monte");
        return false;
    }

    if (!LittleFS.exists(path)) {
        return false;
    }

    File file = LittleFS.open(path, "r");
    if (!file) {
        Serial.printf("[LittleFSManager] Impossible d'ouvrir %s en lecture\n", path.c_str());
        return false;
    }

    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err) {
        Serial.printf("[LittleFSManager] Erreur de parsing JSON sur %s : %s\n",
                      path.c_str(), err.c_str());
        return false;
    }

    return true;
}

bool LittleFSManager::writeJson(const String& path, JsonDocument& doc) {
    if (!_mounted) {
        Serial.println("[LittleFSManager] writeJson: FS non monte");
        return false;
    }

    File file = LittleFS.open(path, "w");
    if (!file) {
        Serial.printf("[LittleFSManager] Impossible d'ouvrir %s en ecriture\n", path.c_str());
        return false;
    }

    size_t written = serializeJson(doc, file);
    file.close();

    return written > 0;
}