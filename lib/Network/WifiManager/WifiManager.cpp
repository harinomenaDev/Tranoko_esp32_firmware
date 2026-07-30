#include "WifiManager.h"



void WifiManager::begin() {

    WiFi.onEvent([this](arduino_event_id_t event, arduino_event_info_t info) {
        handleWifiEvent(event, info);
    });

    WiFi.mode(WIFI_AP);

    WiFi.softAP(Config::AP_SSID, Config::AP_PASSWORD);

    Serial.printf("[WiFi] Access Point Launched As\"%s\" \n IP : %s\n",
                  Config::AP_SSID, WiFi.softAPIP().toString().c_str());
    _connected = true;
    
    if (_onConnected) _onConnected();
}

void WifiManager::onConnected(ConnectedCallback cb) {
    _onConnected = cb;
}

bool WifiManager::isConnected() const {
    return _connected;
}

IPAddress WifiManager::ip() const {
    return WiFi.softAPIP();
}

void WifiManager::handleWifiEvent(arduino_event_id_t event, arduino_event_info_t info) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        {
            uint8_t* mac= info.wifi_ap_staconnected.mac;
            Serial.printf("Client connecté - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        break;
        }

        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:

            break;

        default:
            break;
    }
}

void WifiManager::setCredentials(char* ssid, char* password) {

}

void WifiManager::getCredentials(char*& ssid, char*& password) {

}