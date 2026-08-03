#include "DataPublisher.h"

void DataPublisher::begin(WsServer* wsServer,QueueHandle_t dataQueue) {
    _dataQueue = dataQueue;
    _wsServer = wsServer;
    xTaskCreatePinnedToCore(
        taskEntry,
        "DataPublisherTask",
        Config::PUBLISHER_TASK_STACK,
        this,
        Config::PUBLISHER_TASK_PRIORITY,
        nullptr,
        Config::CORE_APP
    );
}

void DataPublisher::taskEntry(void* pvParams) {
    static_cast<DataPublisher*>(pvParams)->taskLoop();
}

void DataPublisher::taskLoop() {
    DataContent dataContent;
    for (;;) {
        if (xQueueReceive(_dataQueue, &dataContent, portMAX_DELAY) == pdTRUE) {
            switch(dataContent.type) {
                case DEVICE_DATA:
                    _wsServer->sendDataToClients(dataContent.data);

                    Serial.printf("[Data Publisher] Data successfuly pupblished to all clients");
                    break;
                case SYSTEM_DATA:
                    Serial.printf("[Data Publisher] Publishing SYSTEM_DATA: %s\n", dataContent.data.c_str());
                    break;
                case USER_DATA:
                    Serial.printf("[Data Publisher] Publishing USER_DATA: %s\n", dataContent.data.c_str());
                    break;
                default:
                    Serial.printf("[Data Publisher] Unknown data type received: %d\n", dataContent.type);
                    break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

