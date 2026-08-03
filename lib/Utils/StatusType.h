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
constexpr size_t DEVICE_REQUEST_TYPE_LEN = 24;
constexpr size_t DEVICE_ID_LEN           = 32;
constexpr size_t DEVICE_NAME_LEN         = 32;
constexpr size_t DEVICE_TYPE_LEN         = 32;

typedef struct {
    char request_type[DEVICE_REQUEST_TYPE_LEN];
    char id[DEVICE_ID_LEN];
    char name[DEVICE_NAME_LEN];
    char device_type[DEVICE_TYPE_LEN];
    bool status;
    float value;
} DeviceDataType;

typedef enum{
  DEVICE_DATA,
  SYSTEM_DATA,
  USER_DATA,
  UNKNOWN_DATA
}DataType;

typedef struct{
  DataType type;
  String data;
}DataContent;

