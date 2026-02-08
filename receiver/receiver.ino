#include "ESP32_NOW_Serial.h"
#include "MacAddress.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include <Arduino.h>

#define ESPNOW_WIFI_MODE_STATION 1
#define ESPNOW_WIFI_CHANNEL 1

#if ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_STA
#define ESPNOW_WIFI_IF WIFI_IF_STA
#else
#define ESPNOW_WIFI_MODE WIFI_AP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#endif

//B is right motor, A is left motor
#define BIN1_PIN 16   
#define BIN2_PIN 17
#define PWMB_PIN 18 
#define STBY_PIN 47 
#define AIN1_PIN 48 
#define AIN2_PIN 21 
#define PWMA_PIN 15 
#define PWM_FREQ 1000
#define PWM_RES_BITS 8

const MacAddress peer_mac({0x30, 0xED, 0xA0, 0x20, 0xB5, 0xFC}); //this is transmitter mac addr

ESP_NOW_Serial_Class NowSerial(peer_mac, ESPNOW_WIFI_CHANNEL, ESPNOW_WIFI_IF);

struct data_packet
{
  int vrx;
  int vry;
  int turret;
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
  readMacAddress();

  //setup pins for motor control
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMA_PIN, OUTPUT);
  digitalWrite(STBY_PIN, HIGH);
  ledcAttach(PWMA_PIN, PWM_FREQ, PWM_RES_BITS);
  ledcAttach(PWMB_PIN, PWM_FREQ, PWM_RES_BITS);
  
  //start wifi and ESP-NOW 
  WiFi.mode(ESPNOW_WIFI_MODE);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
  while (!(WiFi.STA.started() || WiFi.AP.started()))
  {
    delay(100);
  }

  NowSerial.begin(115200); //start esp now serial communication
}

int remapJoystickValues(int raw, ind mod) {
  int processed = map(abs(raw), 0, 4100, -255, 255)+mod;
  return processed;
}

/*int procLeft(int cx, int cy) {

  int driveLeftSpeed =  constrain(cy + cx, -255, 255)*-1;
  if (abs(cy) >= abs(cx) && cy < 0) {
    // Swap speeds if condition is false
    driveLeftSpeed = *-1;
    Serial.println("REVERSE");
  }
  // Left motor direction
}

int procRight(int cx, int cy) {
  
 // Use xproc and yproc directly for motor control 
  //int driveSpeed =  constrain(cy + cx, -255, 255)*-1;
  int driveRightSpeed = constrain(cy - cx, -255, 255);

  if (abs(cy) >= abs(cx) && cy < 0) {
    // Swap speeds if condition is false
    //driveLeftSpeed = rightSpeed*-1;
    driveRightSpeed = leftSpeed*-1;
    //Serial.println("REVERSE");
  }
  // Left motor direction
}*/
void loop()
{
  if (NowSerial.available())
  {
    NowSerial.readBytes((byte*)&incoming_data, sizeof(incoming_data));
    int xProc = remapJoystickValues(incoming_data.vrx, 10);
    int yProc = remapJoystickValues(incoming_data.vry*-1, -12);
    int tProc = remapJoystickValues(incoming_data.turret, 0);
    Serial.print("Received: ");
    Serial.print(incoming_data.vrx);
    Serial.print(" ");
    Serial.print(incoming_data.vry);
    Serial.print(" ");
    Serial.println(incoming_data.turret);
    Serial.print("Processed: ");
    Serial.print(xProc);
    Serial.print(" ");
    Serial.print(yProc);
    Serial.print(" ");
    Serial.println(tProc);
  }

  delay(45);
}