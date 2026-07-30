#include <Arduino.h>
#include "../Network/WifiManager/WifiManager.h"
#include "../Network/WebsocketManager/WsServer.h"
#include "../Utils/Config.h"
#include "../Hardware/LittleFS/LittleFSManager.h"

static WifiManager     wifiManager;
static WsServer         wsServer;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("System Initialization......");

  wifiManager.onConnected([]() {
    wsServer.begin(Config::WS_PORT, Config::WS_PATH);
  });

  wifiManager.begin();

  FSManager.begin(true);
  
}
void loop(){

}