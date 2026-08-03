#include "WsServer.h"
#include <ArduinoJson.h>
#include "../../Utils/StatusType.h"
#include "../../Security/AESCryptManager/AESCrypt.h"
void WsServer::begin(uint16_t port, const char* path,QueueHandle_t deviceQueue,QueueHandle_t userQueue, QueueHandle_t systemQueue) {
    _server = new AsyncWebServer(port);
    _ws     = new AsyncWebSocket(path);
    _deviceDataQueue = deviceQueue;
    _userDataQueue = userQueue;
    _systemDataQueue = systemQueue;
    _ws->onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client,
                         AwsEventType type, void* arg, uint8_t* data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });
    _server->addHandler(_ws);
    _server->begin();
    Serial.printf("[WsServer] Listening on ws://<device-ip>:%u%s\n", port, path);
}

void WsServer::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                          AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("[WsServer] Client connected from %s\n", client->remoteIP().toString().c_str());
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("[WsServer] Client #%u disconnected\n", client->remoteIP());


            break;

        case WS_EVT_DATA:

            handleFrame(client, data, len);
            break;

        default:
            break;
    }
}

void WsServer::handleFrame(AsyncWebSocketClient* client, uint8_t* data, size_t len) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, data, len);

    if(err) {
        Serial.printf("[WsServer] Failed to parse JSON: %s\n", err.c_str());
        return;
    }

    if(doc["data"].is<String>()){
        String encryptedData = doc["data"].as<String>();
        String decryptedData = AESCrypt::decrypt(encryptedData);

        JsonDocument decryptedDoc;

        DeserializationError decryptErr = deserializeJson(decryptedDoc, decryptedData);
        if(decryptErr) {
            Serial.printf("[WsServer] Failed to parse decrypted JSON: %s\n", decryptErr.c_str());
            return;
        }

        String type = decryptedDoc["type"].as<String>();
        if(type == "DEVICE_CMD_DATA") {
            handleDeviceData(decryptedDoc["data"].as<JsonVariantConst>());
        } else if(type == "USER_CMD_DATA") {
            handleUserData(decryptedDoc["data"].as<JsonVariantConst>());
        } else if(type == "SYSTEM_CMD_DATA") {
            handleSystemData(decryptedDoc["data"].as<JsonVariantConst>());
        } else {
            Serial.printf("[WsServer] Unknown data type: %s\n", type.c_str());
        }
    }
}

void WsServer::handleDeviceData(JsonVariantConst deviceData){
    
DeviceDataType deviceDataStruct = {
    .request_type = deviceData["query"] | "",
    .id           = deviceData["deviceId"] | "",
    .name         = deviceData["name"] | "",
    .device_type  = deviceData["type"] | "",
    .status       = deviceData["status"] | false,
    .value        = deviceData["value"] | 0.0f
};
    xQueueSend(_deviceDataQueue, &deviceDataStruct, portMAX_DELAY);
}

void WsServer::handleSystemData(JsonVariantConst systemData){
    
}

void WsServer::handleUserData(JsonVariantConst userData){

}