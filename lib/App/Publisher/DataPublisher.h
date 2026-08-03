#pragma once

#include <Arduino.h>
#include "../../Utils/Config.h"
#include "../../Utils/StatusType.h"
#include "../../Network/WebsocketManager/WsServer.h"
class DataPublisher {
    public:
        void begin(WsServer* wsServer,QueueHandle_t dataQueue);
    private:
        static void taskEntry(void* pvParams);
        void taskLoop();
        QueueHandle_t _dataQueue;
        WsServer* _wsServer;
};