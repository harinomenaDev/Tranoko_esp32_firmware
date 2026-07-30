#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <functional>
#include "../Utils/Config.h"

class WifiManager {
public:

    using ConnectedCallback = std::function<void()>;

    void begin();

    void onConnected(ConnectedCallback cb);

    bool isConnected() const;

    IPAddress ip() const;

    void setCredentials(char* ssid, char* password);

    void getCredentials(char*& ssid, char*& password);

private:
    void handleWifiEvent(arduino_event_id_t event, arduino_event_info_t info);

    ConnectedCallback _onConnected;
    volatile bool     _connected = false;
};
