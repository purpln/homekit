#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

#define ACCESSORY_NAME  ("sensor")
#define ACCESSORY_SN  ("41745062")
#define ACCESSORY_MANUFACTURER ("homekit")
#define ACCESSORY_MODEL  ("esp8285")

#define pin 2

homekit_characteristic_t name = HOMEKIT_CHARACTERISTIC_(NAME, ACCESSORY_NAME);
homekit_characteristic_t serial_number = HOMEKIT_CHARACTERISTIC_(SERIAL_NUMBER, ACCESSORY_SN);
homekit_characteristic_t door_open_characteristic = HOMEKIT_CHARACTERISTIC_(CONTACT_SENSOR_STATE, 0, .getter=NULL, .setter=NULL, NULL);

void sensor_update() {
  int val = analogRead(pin);
  const uint8_t state = door_open_characteristic.value.uint8_value;
  if(val == 0){
    door_open_characteristic.value = HOMEKIT_UINT8(0);
    homekit_characteristic_notify(&door_open_characteristic, door_open_characteristic.value);
  } else {
    door_open_characteristic.value = HOMEKIT_UINT8(1);
    homekit_characteristic_notify(&door_open_characteristic, door_open_characteristic.value);
  }
}

void sensor_setup(){
  pinMode(pin, INPUT);
}

void door_identify(homekit_value_t _value) {
    printf("Door identifying\n");
}

homekit_accessory_t *accessories[] = {
  HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_programmable_switch, .services=(homekit_service_t*[]){
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]){
      &name,
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
      &serial_number,
      HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, door_identify),
      NULL
    }),
    HOMEKIT_SERVICE(CONTACT_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "sensor"),
      &door_open_characteristic,
      NULL
    }),
    NULL
  }),
  NULL
};

homekit_server_config_t config = {
  .accessories = accessories,
  .password = "111-11-111",
};
