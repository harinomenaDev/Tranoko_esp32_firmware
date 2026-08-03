#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../Utils/StatusType.h"
class DeviceManager {
public:
    void begin(QueueHandle_t deviceQueue);
private:

    static void taskEntry(void* pvParams);
    void taskLoop();
    bool createDevice(const DeviceDataType& device);
    JsonObject findDevice(const char* id);
    bool updateDevice(const DeviceDataType& device);
    bool deleteDevice(const char* id);
    bool deviceExistsIn(JsonArray deviceList, const char* id);
    void printDeviceList();

    JsonArray getDeviceList(); // Assume this function retrieves the existing device list
    JsonDocument devicesDoc;
    QueueHandle_t _deviceQueue;
};
