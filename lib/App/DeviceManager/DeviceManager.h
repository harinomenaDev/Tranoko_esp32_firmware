#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../Utils/StatusType.h"
#include "../../Hardware/LittleFS/LittleFSManager.h"
#include "../../Network/WebsocketManager/WsServer.h"

class DeviceManager {
public:
    void begin(QueueHandle_t deviceQueue, LittleFSManager& fsManager,QueueHandle_t dataQueue);
private:

    static void taskEntry(void* pvParams);
    void taskLoop();
    bool createDevice(const DeviceDataType& device);
    JsonObject findDevice(const char* id);
    bool updateDevice(const DeviceDataType& device);
    bool deleteDevice(const char* id);
    bool deviceExistsIn(JsonArray deviceList, const char* id);
    void printDeviceList();
    void publishDeviceList();

    JsonArray getDeviceList(); // charge depuis le FS au premier appel, garantit un JsonArray valide
    bool saveDevices(); // persiste devicesDoc sur le filesystem
    JsonDocument devicesDoc;
    bool _loaded = false;
    QueueHandle_t _deviceQueue;
    QueueHandle_t _dataQueue;

    LittleFSManager* _fsManager;
};