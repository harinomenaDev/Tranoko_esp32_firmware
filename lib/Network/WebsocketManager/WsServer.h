#pragma once
#include <ESPAsyncWebServer.h>
class WsServer {
public:
    void begin(uint16_t port, const char* path,
                QueueHandle_t deviceQueue,QueueHandle_t userQueue, QueueHandle_t systemQueue);

private:
    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                   AwsEventType type, void* arg, uint8_t* data, size_t len);

    void handleFrame(AsyncWebSocketClient* client, uint8_t* data, size_t len);
    void handleDeviceData(JsonDocument deviceData);
    void handleUserData(JsonDocument UserData);
    void handleSystemData(JsonDocument systemData);

    AsyncWebServer* _server = nullptr;
    AsyncWebSocket* _ws     = nullptr;
    
    QueueHandle_t _deviceDataQueue;
    QueueHandle_t _userDataQueue;
    QueueHandle_t _systemDataQueue;
};
