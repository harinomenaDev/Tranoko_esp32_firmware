#pragma once
#include <ESPAsyncWebServer.h>
class WsServer {
public:
    void begin(uint16_t port, const char* path);

private:
    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                   AwsEventType type, void* arg, uint8_t* data, size_t len);

    void handleFrame(AsyncWebSocketClient* client, uint8_t* data, size_t len);

    AsyncWebServer* _server = nullptr;
    AsyncWebSocket* _ws     = nullptr;

};
