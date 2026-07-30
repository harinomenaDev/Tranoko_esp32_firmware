#include "WsServer.h"
#include <ArduinoJson.h>

void WsServer::begin(uint16_t port, const char* path) {
    _server = new AsyncWebServer(port);
    _ws     = new AsyncWebSocket(path);

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
            Serial.printf("[WsServer] Client #%u connected from %s\n",
                          client->id(), client->remoteIP().toString().c_str());
            client->client()->setNoDelay(true); 


            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("[WsServer] Client #%u disconnected\n", client->id());


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
    if (err) {
        Serial.printf("[WsServer] Bad JSON from #%u: %s\n", client->id(), err.c_str());
        return;
    }

}

