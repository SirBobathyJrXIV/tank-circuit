#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"
#include "esp_wifi.h"

#define ESPNOW_WIFI_MODE_STATION 1
#define ESPNOW_WIFI_CHANNEL 1

#if ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_STA
#define ESPNOW_WIFI_IF WIFI_IF_STA
#else
#define ESPNOW_WIFI_MODE WIFI_AP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#endif
//*/
// Set the MAC address of the device that will transmit the data
const MacAddress peer_mac({0x30, 0xED, 0xA0, 0x20, 0xB5, 0xFC}); //this is transmitter mac addr

ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

struct data_packet
{
  int value1;
  int value2;
  int value3;
};

data_packet incoming_data { 0, 0, 0 };

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(ESPNOW_WIFI_MODE);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

  while (!(WiFi.STA.started() || WiFi.AP.started()))
  {
    delay(100);
  }

  NowSerial.begin(115200);
  readMacAddress();
}

void loop()
{
  if (NowSerial.available())
  {
    NowSerial.readBytes((byte*)&incoming_data, sizeof(incoming_data));
    Serial.print(incoming_data.value1);
    Serial.print(" ");
    Serial.print(incoming_data.value2);
    Serial.print(" ");
    Serial.println(incoming_data.value3);
  }
  //else Serial.println("Did not receive any data");

  delay(1);
}