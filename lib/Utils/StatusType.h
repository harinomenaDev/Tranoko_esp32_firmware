#pragma once

/*
{
  "data" : "Cyphertext",
}
*/

/*
  Decrypted data format:
  { 
    "type" : "DEVICE_CMD_DATA",
    "data" : {
      "id" : "device_id",
      "name" : "device_name",
      "device_type" : "device_type",
      "state" : "device_state"
    }
  }
*/

typedef struct{
    const char* request_type;
    const char* id;
    const char* name;
    const char* device_type;
    bool status;
    float value;
}DeviceDataType;

