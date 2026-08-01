#pragma once

/*
{
  "type": "DEVICE_CMD_DATA",
  "data": {
    "count": 3,
    "devices": [
      {
        "id": "gpio_001",
        "name": "LED Salon",
        "device_type": "gpio",
        "state": "on"
      },
      {
        "id": "sensor_001",
        "name": "Temperature Salon",
        "device_type": "sensor",
        "state": "active",
        "value": 24.5,
        "unit": "C"
      },
      {
        "id": "pwm_001",
        "name": "Ventilateur",
        "device_type": "pwm",
        "state": "on",
        "duty_cycle": 60
      }
    ]
  }
}
*/

typedef enum{
    DEVICE_CMD_DATA,
    USER_CMD_DATA,
    SYSTEM_CMD_DATA,
    UNKNOWN_CMD_DATA
}ClientDataType;

typedef struct{
    char* id;
    char* name;
    char* device_type;
    char* state;
}GPIODeviceData;

typedef enum{
    CREATE_DEVICE,
    READ_DEVICE,
    UPDATE_DEVICE,
    DELETE_DEVICE,
    UNKNOWN_DEVICE_REQUEST
}DeviceRequestType;