#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

#define ACCESSORY_NAME  ("outlet")
#define ACCESSORY_SN  ("41745062")
#define ACCESSORY_MANUFACTURER ("homekit")
#define ACCESSORY_MODEL  ("esp8285")

#define pin 2

bool outlet_power = false;

homekit_value_t outlet_on_get() {
  return HOMEKIT_BOOL(outlet_power);
}

void outlet_on_set(homekit_value_t value) {
  if (value.format != homekit_format_bool) {
    printf("Invalid on-value format: %d\n", value.format);
    return;
  }
  outlet_power = value.bool_value;
  outlet_update();
}

homekit_characteristic_t name = HOMEKIT_CHARACTERISTIC_(NAME, ACCESSORY_NAME);
homekit_characteristic_t serial_number = HOMEKIT_CHARACTERISTIC_(SERIAL_NUMBER, ACCESSORY_SN);
homekit_characteristic_t outlet_on = HOMEKIT_CHARACTERISTIC_(ON, false, .getter = outlet_on_get, .setter = outlet_on_set);

void outlet_update() {
  if (outlet_power) {
    printf("on\n");
    digitalWrite(pin, HIGH);
  } else {
    printf("off\n");
    digitalWrite(pin, LOW);
  }
}

void pin_toggle() {
  outlet_on.value.bool_value = !outlet_on.value.bool_value;
  outlet_on.setter(outlet_on.value);
  homekit_characteristic_notify(&outlet_on, outlet_on.value);
}

void accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
  for (int j = 0; j < 3; j++) {
    outlet_power = true;
    outlet_update();
    delay(100);
    outlet_power = false;
    outlet_update();
    delay(100);
  }
}

homekit_accessory_t *accessories[] = {
  HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_outlet, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      &name,
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
      &serial_number,
      HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
      NULL
    }),
    HOMEKIT_SERVICE(OUTLET, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "outlet"),
      &outlet_on,
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

void accessory_init() {
  pinMode(pin, OUTPUT);
  outlet_update();
}
