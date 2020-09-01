#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

#define ACCESSORY_NAME  ("temperature")
#define ACCESSORY_SN  ("41745062")
#define ACCESSORY_MANUFACTURER ("homekit")
#define ACCESSORY_MODEL  ("esp8285")

homekit_characteristic_t name = HOMEKIT_CHARACTERISTIC_(NAME, ACCESSORY_NAME);
homekit_characteristic_t serial_number = HOMEKIT_CHARACTERISTIC_(SERIAL_NUMBER, ACCESSORY_SN);
homekit_characteristic_t temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
float temperature_value;
extern float t;

void temp_update() {
  temperature_value = t;
  temperature.value.float_value = temperature_value;
  *temperature.min_value = -200;
  *temperature.max_value = 200;
  homekit_characteristic_notify(&temperature, HOMEKIT_FLOAT(temperature_value));
  delay(1000);
}

void temperature_sensor_identify(homekit_value_t _value) {
  printf("Temperature sensor identify\n");
}

homekit_accessory_t *accessories[] = {
  HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_thermostat, .services=(homekit_service_t*[]){
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]){
      &name,
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
      &serial_number,
      HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, temperature_sensor_identify),
      NULL
    }),
    HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "temperature sensor"),
      &temperature,
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
