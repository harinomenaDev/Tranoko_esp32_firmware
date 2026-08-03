#include <Arduino.h>
#include "../Utils/Config.h"
#include "../Utils/StatusType.h"
#include "../Hardware/LittleFS/LittleFSManager.h"

#include "../Network/WifiManager/WifiManager.h"
#include "../Network/WebsocketManager/WsServer.h"

#include "../App/DeviceManager/DeviceManager.h"


static WifiManager     wifiManager;
static WsServer         wsServer;
static DeviceManager    deviceManager;

QueueHandle_t deviceDataQueue;
QueueHandle_t userDataQueue;
QueueHandle_t systemDataQueue;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("System Initialization......");

  deviceDataQueue = xQueueCreate(Config::deviceQueueLengh, sizeof(DeviceDataType));


  FSManager.begin(true);

  deviceManager.begin(deviceDataQueue);
  
  wifiManager.onConnected([]() {
    wsServer.begin(Config::WS_PORT, Config::WS_PATH,deviceDataQueue,userDataQueue,systemDataQueue);
  });

  wifiManager.begin();
  
}
void loop(){

}