#line 1 "C:\\Users\\phamw\\Desktop\\tank-circuit\\tank-circuit.ino"
#include <Arduino.h>

/* ----- Pin Definitions (TB6612FNG driver + joystick) ----- */
#define BIN1_PIN 16   // B_IN1 (right motor direction)
#define BIN2_PIN 17   // B_IN2 (right motor direction)
#define PWMB_PIN 18   // PWMB  (right motor PWM)

#define STBY_PIN 47   // STBY  (stand-by enable)

#define AIN1_PIN 48   // A_IN1 (left motor direction)
#define AIN2_PIN 21   // A_IN2 (left motor direction)
#define PWMA_PIN 15    // PWMA  (left motor PWM)

#define VRX_PIN 4     // Joystick X-axis (ADC)
#define VRY_PIN 5     // Joystick Y-axis (ADC)
/* ---------------------------------------------------------- */

#define PWM_FREQ      1000
#define PWM_RES_BITS  8

#line 21 "C:\\Users\\phamw\\Desktop\\tank-circuit\\tank-circuit.ino"
void setup();
#line 45 "C:\\Users\\phamw\\Desktop\\tank-circuit\\tank-circuit.ino"
void loop();
#line 138 "C:\\Users\\phamw\\Desktop\\tank-circuit\\tank-circuit.ino"
void printMotorBar(const char* label, int speed);
#line 21 "C:\\Users\\phamw\\Desktop\\tank-circuit\\tank-circuit.ino"
void setup() {
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(STBY_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMA_PIN, OUTPUT);

  digitalWrite(STBY_PIN, HIGH);  // Enable motors

  // Setup PWM using ESP32 3.x API
  ledcAttach(PWMA_PIN, PWM_FREQ, PWM_RES_BITS);
  ledcAttach(PWMB_PIN, PWM_FREQ, PWM_RES_BITS);
  

  Serial.begin(115200);
 // while (!Serial) { /* wait for USB-CDC on some boards */ }

  Serial.println("ESP32-S3 joystick monitor");
  Serial.println("Raw ADC      |  Centered (−2048…+2047)");
  Serial.println("---------------------------------------");
}

void loop() {
  // Read raw 12-bit ADC values (0…4095)
  int rawX = analogRead(VRX_PIN);
  int rawY = analogRead(VRY_PIN);
  int cx = map(abs(rawX), 0, 4100, -255, 255)+10;
  int cy = map(abs(rawY), 0, 4100, 255, -255)-12;

  // Print to Serial
  Serial.println("================");
  Serial.print("X: ");
  Serial.print(rawX);
  Serial.print("  Y: ");
  Serial.println(rawY);
  Serial.print("cX: ");
  Serial.print(cx);
  Serial.print("  cY: ");
  Serial.println(cy);

  // Use cx and cy directly for motor control (tank-style mix)
  int leftSpeed = constrain(cy + cx, -255, 255)*-1;
  int rightSpeed = constrain(cy - cx, -255, 255);

  // Left motor direction
  //if ()
  if (cx < 25 || (cy < 45 && cy > -45)){

  if (leftSpeed > 5) {
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
  } else if (leftSpeed < -5) {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
  } else {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, LOW);
  }

  // Right motor direction
  if (rightSpeed > 5) {
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
  } else if (rightSpeed < -5) {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  } else {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, LOW);
  }
  } 
  
  else
  { //back left and back right
    if (rightSpeed > 5) {
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
  } else if (rightSpeed < -5) {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
  } else {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, LOW);
  }

  // Right motor direction
  if (leftSpeed > 5) {
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
  } else if (leftSpeed < -5) {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  } else {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, LOW);
  }
  }

  // Set PWM
  ledcWrite(PWMA_PIN, abs(leftSpeed));
  ledcWrite(PWMB_PIN, abs(rightSpeed));
  printMotorBar("RS", rightSpeed*-1);
  printMotorBar("LS", leftSpeed);


  // Print power applied to motors
  Serial.print("L PWM: ");
  Serial.print(leftSpeed*-1);
  Serial.print("  R PWM: ");
  Serial.println(rightSpeed);



  delay(50);  
}
void printMotorBar(const char* label, int speed) {
  const int barLength = 13;  // Total characters in the bar
  char bar[barLength + 1];
  memset(bar, '.', barLength);
  bar[barLength] = '\0';

  int blocks = map(abs(speed), 0, 255, 0, barLength);

  if (speed > 0) {
    for (int i = 0; i < blocks; i++) bar[i] = '>';
  } else if (speed < 0) {
    for (int i = 0; i < blocks; i++) bar[barLength - 1 - i] = '<';
  }

  Serial.print(label);
  Serial.print("[");
  Serial.print(bar);
  Serial.println("]");
}


