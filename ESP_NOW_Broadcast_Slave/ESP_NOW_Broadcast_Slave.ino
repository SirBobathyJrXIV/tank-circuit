#include <esp_now.h>

#include <WiFi.h>



typedef struct struct_message {

    char deviceName[32];

    int sensorValue;

    float temperature;

    bool status;

    unsigned long timestamp;

} struct_message;



struct_message incomingMessage;



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    

    Serial.println("=== Message Received ===");

    Serial.printf("Device: %s\n", incomingMessage.deviceName);

    Serial.printf("Sensor: %d\n", incomingMessage.sensorValue);

    Serial.printf("Temperature: %.2f°C\n", incomingMessage.temperature);

    Serial.printf("Status: %s\n", incomingMessage.status ? "Active" : "Inactive");

    Serial.printf("Timestamp: %lu\n", incomingMessage.timestamp);

    Serial.println("========================");

}



void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    Serial.println(WiFi.macAddress());

    

    if (esp_now_init() != ESP_OK) {

        Serial.println("Error initializing ESP-NOW");

        return;

    }

    

//    esp_now_register_recv_cb(OnDataRecv);

    Serial.println("ESP-NOW Receiver Ready");

}



void loop() {

    delay(1000);

}