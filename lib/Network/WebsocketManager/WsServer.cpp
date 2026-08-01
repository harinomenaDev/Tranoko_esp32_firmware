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
            Serial.printf("[WsServer] Client connected from %s\n",
                          client->id(), client->remoteIP().toString().c_str());
            client->client()->setNoDelay(true); 


            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("[WsServer] Client #%u disconnected\n", client->remoteIP());


            break;

        case WS_EVT_DATA:
            /*Decryption*/
            handleFrame(client, data, len);
            break;

        default:
            break;
    }
}

void WsServer::handleFrame(AsyncWebSocketClient* client, uint8_t* data, size_t len) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, data, len);
    if (err) {
        Serial.printf("[WsServer] Bad JSON from #%u: %s\n", client->id(), err.c_str());
        return;
    }
    if(doc["type"].is<const char*>()){     

        const char* type = doc["type"];

        if(strcmp(type, "DEVICE_CMD_DATA") == 0){
            Serial.printf("[WsServer] Received DEVICE_CMD_DATA from #%u\n", client->id());

            if(doc["data"].is<JsonDocument>()){              
                handleDeviceData(doc["data"]);
            }

            
        } else if(strcmp(type, "USER_CMD_DATA") == 0){
            Serial.printf("[WsServer] Received USER_CMD_DATA from #%u\n", client->id());

            if(doc["data"].is<JsonDocument>()){              
                handleUserData(doc["data"]);
            }

        } else if(strcmp(type, "SYSTEM_CMD_DATA") == 0){
            Serial.printf("[WsServer] Received SYSTEM_CMD_DATA from #%u\n", client->id());

            if(doc["data"].is<JsonDocument>()){              
                handleSystemData(doc["data"]);
            }
            
        } else {
            Serial.printf("[WsServer] Unknown type from #%u: %s\n", client->id(), type);
        }
    } else {
        Serial.printf("[WsServer] No type field in JSON from #%u\n", client->id());
    }

}

void WsServer::handleDeviceData(JsonDocument deviceData){

}

void WsServer::handleSystemData(JsonDocument systemData){
    
}

void WsServer::handleUserData(JsonDocument userData){
    
}