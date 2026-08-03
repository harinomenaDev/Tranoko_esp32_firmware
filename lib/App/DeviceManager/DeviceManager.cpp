#include "DeviceManager.h"
#include "../../Utils/Config.h"
#include "../../Utils/Utils.h"
void DeviceManager::begin(QueueHandle_t deviceQueue, LittleFSManager& fsManager, QueueHandle_t dataQueue) {
    _deviceQueue = deviceQueue;
    _fsManager = &fsManager;
    _dataQueue = dataQueue;
    _fsManager->createEmptyJsonFile(Config::deviceJSONPath);
    xTaskCreatePinnedToCore(
        taskEntry,
        "DeviceMgrTask",
        Config::DEVICE_MANAGER_TASK_STACK,
        this,
        Config::DEVICE_MANAGER_TASK_PRIORITY,
        nullptr,
        Config::CORE_APP
    );
}

void DeviceManager::taskEntry(void* pvParams) {
    static_cast<DeviceManager*>(pvParams)->taskLoop();
}

void DeviceManager::taskLoop() {
    DeviceDataType deviceData;
    DataContent dataContent;
    dataContent.type = DEVICE_DATA;
    for (;;) {
        if (xQueueReceive(_deviceQueue, &deviceData, portMAX_DELAY) == pdTRUE) {
            if(deviceData.request_type[0] == '\0') {
                Serial.println("[Device Manager] Received device data with empty request_type");
                continue;
            }else if(strcmp(deviceData.request_type, "CREATE_DEVICE") == 0) {
                if(createDevice(deviceData)) {
                    Serial.printf("[Device Manager] Device created: id=%s, name=%s, type=%s\n", deviceData.id, deviceData.name, deviceData.device_type);    

                } else {
                    // Serial.printf("[Device Manager] Failed to create device: id=%s\n", deviceData.id);
                }
            } else if(strcmp(deviceData.request_type, "UPDATE_DEVICE") == 0){
                if(updateDevice(deviceData)) {
                    Serial.printf("[Device Manager] Device updated: id=%s\n", deviceData.id);
                } else {
                    // Serial.printf("[Device Manager] Failed to update device: id=%s\n", deviceData.id);
                }
            } else if(strcmp(deviceData.request_type, "DELETE_DEVICE") == 0){
                if(deleteDevice(deviceData.id)) {
                    Serial.printf("[Device Manager] Device deleted: id=%s\n", deviceData.id);
                } else {
                    // Serial.printf("[Device Manager] Failed to delete device: id=%s\n", deviceData.id);
                }
            }else if(strcmp(deviceData.request_type, "PRINT_DEVICE") == 0){
                printDeviceList();
            }
             else {
                Serial.printf("[Device Manager] Unknown request type: %s\n", deviceData.request_type);
            }

            
        }
            vTaskDelay(pdMS_TO_TICKS(2));

    }
}
JsonObject DeviceManager::findDevice(const char* id) {
    JsonArray deviceList = getDeviceList();

    for (JsonObject device : deviceList) {
        if (strcmp(device["id"], id) == 0) {
            return device;
        }
    }

    return JsonObject();
}

bool DeviceManager::updateDevice(const DeviceDataType& device) {
    JsonArray deviceList = getDeviceList();
    if(!deviceExistsIn(deviceList, device.id)) {
        Serial.printf("[Device Manager] Device with id=%s does not exist. Cannot update.\n", device.id);
        return false;
    }

    if(isBlank(device.id) || isBlank(device.name) || isBlank(device.device_type)) {
        Serial.println("[Device Manager] Invalid device data. Missing id, name, or type.");
        return false;
    }

    for (JsonObject obj : deviceList) {
        if (strcmp(obj["id"], device.id) == 0) {
            obj["name"] = String(device.name);
            obj["type"] = String(device.device_type);
            obj["status"] = device.status;
            obj["value"] = device.value;
            return saveDevices();
        }
    }

    return false;
}

bool DeviceManager::deleteDevice(const char* id) {
    JsonArray deviceList = getDeviceList();

    for (size_t i = 0; i < deviceList.size(); i++) {
        if (strcmp(deviceList[i]["id"], id) == 0) {
            deviceList.remove(i);
            return saveDevices();
        }
    }
        Serial.printf("[Device Manager] Device with id=%s does not exist. Cannot delete.\n", id);

    return false;
}


bool DeviceManager::createDevice(const DeviceDataType& device) {
    JsonArray deviceList = getDeviceList();

    if(deviceExistsIn(deviceList, device.id)) {
        Serial.printf("[Device Manager] Device with id=%s already exists. Skipping creation.\n", device.id);
        return false;
    }
    if(deviceList.size() >= Config::MAX_DEVICES) {
        Serial.println("[Device Manager] Maximum device limit reached. Cannot create new device.");
        return false;
    }
    if(isBlank(device.id) || isBlank(device.name) || isBlank(device.device_type)) {
        Serial.println("[Device Manager] Invalid device data. Missing id, name, or type.");
        return false;
    }

    JsonObject obj = deviceList.add<JsonObject>();
    obj["id"] = String(device.id);
    obj["name"] = String(device.name);
    obj["type"] = String(device.device_type);
    obj["status"] = device.status;
    obj["value"] = device.value;

    return saveDevices();
}

JsonArray DeviceManager::getDeviceList() {
    if (!_loaded) {
        _fsManager->readJson(Config::deviceJSONPath, devicesDoc);
        _loaded = true;
    }

    JsonVariant list = devicesDoc["deviceList"];
    return list.is<JsonArray>() ? list.as<JsonArray>() : list.to<JsonArray>();
}

bool DeviceManager::saveDevices() {
    bool success = _fsManager->writeJson(Config::deviceJSONPath, devicesDoc);
    if(success) {
        Serial.println("[Device Manager] Device list saved successfully.");
        publishDeviceList(); // Publish the updated device list after saving
        return true;
    } 
    return false;
}

void DeviceManager::printDeviceList() {
    JsonArray deviceList = getDeviceList();
    Serial.println("=== Device List ===");
    serializeJsonPretty(deviceList, Serial);
    Serial.println();
}

bool DeviceManager::deviceExistsIn(JsonArray deviceList, const char* id) {

    for (JsonObject device : deviceList) {
        const char* deviceId = device["id"];
            if (strcmp(deviceId, id) == 0) {
            return true;
        }
    }

    return false;
}

void DeviceManager::publishDeviceList() {
    JsonArray deviceList = getDeviceList();
    DataContent dataContent;
    dataContent.type = DEVICE_DATA;
    serializeJson(deviceList, dataContent.data);
    xQueueSend(_dataQueue, &dataContent, portMAX_DELAY);
}