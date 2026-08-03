#include <Arduino.h>
#include "../Utils/Config.h"
#include "../Utils/StatusType.h"
#include "../Hardware/LittleFS/LittleFSManager.h"

#include "../Network/WifiManager/WifiManager.h"
#include "../Network/WebsocketManager/WsServer.h"

#include "../App/DeviceManager/DeviceManager.h"
#include "../App/Publisher/DataPublisher.h"


static WifiManager     wifiManager;
static WsServer         wsServer;
static DeviceManager    deviceManager;
static DataPublisher     dataPublisher;


QueueHandle_t deviceDataQueue;
QueueHandle_t userDataQueue;
QueueHandle_t systemDataQueue;

QueueHandle_t dataQueue;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("System Initialization......");
  deviceDataQueue = xQueueCreate(Config::deviceQueueLengh, sizeof(DeviceDataType));
  dataQueue = xQueueCreate(Config::dataQueueLength, sizeof(DataContent));
  FSManager.begin(true);
  wifiManager.onConnected([]() {
    wsServer.begin(Config::WS_PORT, Config::WS_PATH, FSManager, deviceDataQueue, userDataQueue, systemDataQueue);
  });
  wifiManager.begin();
  dataPublisher.begin(&wsServer,dataQueue);
  deviceManager.begin(deviceDataQueue, FSManager,dataQueue);  
}
void loop(){

}