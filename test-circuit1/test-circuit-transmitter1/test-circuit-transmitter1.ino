#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"
#include "esp_wifi.h"

#define ESPNOW_WIFI_MODE_STATION 1
#define ESPNOW_WIFI_CHANNEL 1
#define ESPNOW_WIFI_MODE WIFI_STA
#define ESPNOW_WIFI_IF WIFI_IF_STA

const MacAddress peer_mac({0xF0, 0xF5, 0xBD, 0xFD, 0x59, 0x8C});
ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

struct data_packet
{
  int value1;
  int value2;
  int value3;
};
data_packet outgoing_data = { 0, 0, 0 };

unsigned long last_send_time = 0;
const unsigned long SEND_INTERVAL = 1000;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(100);
  }
  
  WiFi.mode(ESPNOW_WIFI_MODE);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  
  while (!(WiFi.STA.started() || WiFi.AP.started()))
  {
    delay(100);
  }
  
  NowSerial.begin(115200);
  Serial.println("Setup complete");
}

void loop()
{
  unsigned long current_time = millis();
  
  if (current_time - last_send_time >= SEND_INTERVAL)
  {
    // Read your three integer values here
    int value1 = 21; // Replace with your actual value
    int value2 = 42; // Replace with your actual value
    int value3 = 100; // Replace with your actual value
    
    outgoing_data = { value1, value2, value3 };
    NowSerial.write((byte*)&outgoing_data, sizeof(outgoing_data));
    Serial.println("Transmitted?");
    
    last_send_time = current_time;
  }
  
  delay(50);
}