#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <arduino_homekit_server.h>
#define SIMPLE_INFO(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
const char *ssid = "ssid";
const char *password = "password";

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define term_vcc 2
#define term_gnd 4
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float t = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.setRxBufferSize(32);
  Serial.setDebugOutput(false);
  sensors.begin();
  pinMode(term_vcc, OUTPUT);
  pinMode(term_gnd, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.disconnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  SIMPLE_INFO("");
  SIMPLE_INFO("SketchSize: %d", ESP.getSketchSize());
  SIMPLE_INFO("FreeSketchSpace: %d", ESP.getFreeSketchSpace());
  SIMPLE_INFO("FlashChipSize: %d", ESP.getFlashChipSize());
  SIMPLE_INFO("FlashChipRealSize: %d", ESP.getFlashChipRealSize());
  SIMPLE_INFO("FlashChipSpeed: %d", ESP.getFlashChipSpeed());
  SIMPLE_INFO("SdkVersion: %s", ESP.getSdkVersion());
  SIMPLE_INFO("FullVersion: %s", ESP.getFullVersion().c_str());
  SIMPLE_INFO("CpuFreq: %dMHz", ESP.getCpuFreqMHz());
  SIMPLE_INFO("FreeHeap: %d", ESP.getFreeHeap());
  SIMPLE_INFO("ResetInfo: %s", ESP.getResetInfo().c_str());
  SIMPLE_INFO("ResetReason: %s", ESP.getResetReason().c_str());
  INFO_HEAP();
  homekit_setup();
  INFO_HEAP();
}

void loop() {
  homekit_loop();
}

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t name;
extern "C" void temp_update();

void homekit_setup() {
  //homekit_storage_reset();
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  int name_len = snprintf(NULL, 0, "%s_%02X%02X%02X", name.value.string_value, mac[3], mac[4], mac[5]);
  char *name_value = (char*) malloc(name_len + 1);
  snprintf(name_value, name_len + 1, "%s_%02X%02X%02X", name.value.string_value, mac[3], mac[4], mac[5]);
  name.value = HOMEKIT_STRING_CPP(name_value);
  arduino_homekit_setup(&config);
}

void homekit_loop() {
  temperatures();
  temp_update();
  arduino_homekit_loop();
  static uint32_t next_heap_millis = 0;
  uint32_t time = millis();
  if (time > next_heap_millis) {
    SIMPLE_INFO("heap: %d, sockets: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
    next_heap_millis = time + 5000;
  }
}

void temperatures() {
  digitalWrite(term_gnd, LOW);
  digitalWrite(term_vcc, HIGH);
  delay(100);
  sensors.requestTemperatures();
  delay(500);
  sensors.requestTemperatures();
  t = float(sensors.getTempCByIndex(0));
  t = t + 0.0;
  digitalWrite(term_vcc, LOW);
  delay(1400);
}
