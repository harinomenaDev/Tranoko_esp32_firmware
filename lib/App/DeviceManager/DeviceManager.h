#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../Utils/StatusType.h"
#include "../../Hardware/LittleFS/LittleFSManager.h"
class DeviceManager {
public:
    void begin(QueueHandle_t deviceQueue, LittleFSManager& fsManager);
private:

    static void taskEntry(void* pvParams);
    void taskLoop();
    bool createDevice(const DeviceDataType& device);
    JsonObject findDevice(const char* id);
    bool updateDevice(const DeviceDataType& device);
    bool deleteDevice(const char* id);
    bool deviceExistsIn(JsonArray deviceList, const char* id);
    void printDeviceList();

    JsonArray getDeviceList(); // charge depuis le FS au premier appel, garantit un JsonArray valide
    bool saveDevices(); // persiste devicesDoc sur le filesystem
    JsonDocument devicesDoc;
    bool _loaded = false;
    QueueHandle_t _deviceQueue;

    LittleFSManager* _fsManager;
};