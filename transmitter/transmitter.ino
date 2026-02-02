#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi_types.h>  // For wifi_tx_info_t

// Receiver MAC address
uint8_t receiverMAC[] = {0x98, 0x3D, 0xAE, 0x60, 0xCD, 0x78};

typedef struct struct_message {
  int16_t vrx;
  int16_t vry;
} struct_message;

struct_message joystickData;

// Updated callback to use src_addr instead of mac_addr
void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("Last Packet Sent to: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", tx_info->src_addr[i]);  // Changed from mac_addr to src_addr
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read joystick analog values
  joystickData.vrx = analogRead(4);  // Example VRX pin
  joystickData.vry = analogRead(5);  // Example VRY pin

  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&joystickData, sizeof(joystickData));

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending data");
  }
  delay(100);
}