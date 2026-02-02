
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Define the structure for the incoming message, matching the transmitter
typedef struct struct_message {
  int16_t vrx;
  int16_t vry;
} struct_message;

// Variable to store incoming data
struct_message incomingData;

// Callback function for when data is received
void onDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingDataPtr, int len) {
  // Verify the received data size matches the expected structure
  if (len == sizeof(struct_message)) {
    memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
    Serial.print("Received VRX: ");
    Serial.print(incomingData.vrx);
    Serial.print(" | VRY: ");
    Serial.println(incomingData.vry);
  } else {
    Serial.println("Error: Received data size mismatch");
  }
}

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(1000);  // Small delay to ensure Serial is ready

  // Set WiFi to Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    while (true)
      ;  // Halt on failure
  }

  // Register the callback function for received data
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("ESP-NOW Receiver Initialized");
}

void loop() {
  // No code needed in loop; everything is handled by the callback
}

/*
### Changes Made
1. **Updated Callback Signature**:
   - Changed the `onDataRecv` function signature to `void onDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingDataPtr, int len)` to match the expected type `esp_now_recv_cb_t` in the ESP32 Arduino core version 3.3.0.
   - The `esp_now_recv_info` structure includes the sender's MAC address (`src_addr`), receiver's MAC address (`des_addr`), and other metadata. In this case, we only need the data (`incomingDataPtr`) and length (`len`), so the `recv_info` parameter is included but not used.

2. **Preserved Functionality**:
   - The rest of the code remains unchanged, as it correctly handles the reception of `vrx` and `vry` values and prints them to the Serial monitor.
   - The `struct_message` definition matches the transmitter's structure to ensure proper data alignment.
   - Error handling for data size mismatches and ESP-NOW initialization failures is retained.

### Build Configuration
Your build log shows the following configuration for the ESP32-S3:
- **FQBN**: `esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=8M,PSRAM=enabled`
- **Board**: ESP32S3_DEV
- **Core**: ESP32 Arduino core version 3.3.0
- **Tools**: esptool v5.0.0, xtensa-esp32s3-elf-g++

This code is compatible with your setup, as it uses the standard Arduino libraries (`Arduino.h`, `esp_now.h`, `WiFi.h`) and accounts for the updated ESP-NOW API in version 3.3.0.

### Notes
- **MAC Address**: Ensure the `receiverMAC` in the transmitter code (`98:3D:AE:60:CD:78`) matches the MAC address of your ESP32-S3 receiver. You can print the receiver's MAC address by adding `Serial.println(WiFi.macAddress());` in the `setup()` function to verify.
- **WiFi Channel**: The transmitter uses channel 0 (default). The receiver will automatically listen on the same channel as configured by ESP-NOW.
- **Serial Monitor**: Open the Serial Monitor in the Arduino IDE at 115200 baud to view the output (`Received VRX: <value> | VRY: <value>`).
- **Testing**: Upload the corrected receiver code to your ESP32-S3, ensure the transmitter is running, and check the Serial Monitor for the `vrx` and `vry` values sent every ~100ms.

### Troubleshooting
If you still encounter issues:
1. **Verify ESP32 Core Version**: Ensure you are using ESP32 Arduino core version 3.3.0, as indicated in your build log. If you have a different version, the callback signature might differ.
2. **Check MAC Address**: If no data is received, verify the MAC address in the transmitter code.
3. **Debug Output**: Add `Serial.println(WiFi.macAddress());` in the receiver’s `setup()` to confirm the MAC address.
4. **Channel Mismatch**: Ensure no other WiFi networks are interfering. You can explicitly set the channel in the receiver by adding `WiFi.channel(0);` before `esp_now_init()` if needed.

This corrected code should compile successfully and work with your transmitter to receive and print `vrx` and `vry` values. Let me know if you need further assistance!*/