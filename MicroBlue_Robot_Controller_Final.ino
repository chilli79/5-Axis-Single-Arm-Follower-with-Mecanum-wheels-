#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h> 
#include <string>

// ===================================
// 1. PIN DEFINITIONS
// ===================================

// --- SERVOS (Robot Arm) ---
// Controlled by MicroBlue Sliders (sl0, sl1, sl2) and Button (b0)
Servo baseServo;   // GPIO 18 (Arm Base Rotation) -> sl0
Servo elbowServo;  // GPIO 16 (Arm Elbow) -> sl1
Servo wristServo;  // GPIO 17 (Arm Wrist) -> sl2
Servo jawServo;    // GPIO 23 (Arm Gripper/Jaw) -> b0

// Initial Arm State: "Rest" Position
int baseAngle = 180;   // Initial base rotation is 180 (Fully turned)
int elbowAngle = 90;   // Initial elbow position 90 (Neutral rest)
int wristAngle = 90;   // Initial wrist position 90 (Neutral)
int jawAngle = 120;    // Initial jaw position 120 (Open)

// Gripper/Jaw specific angles
const int JAW_OPEN_ANGLE = 120; 
const int JAW_CLOSE_ANGLE = 0;  

// --- L298N MOTOR CONTROL (Mobile Base) ---
#define MOTOR_IN1  26 
#define MOTOR_IN2  25 
#define MOTOR_IN3  19 
#define MOTOR_IN4  5  
#define MOTOR_EN_A 33 
#define MOTOR_EN_B 2  

// *** MAXIMUM SPEED SETTING (255) ***
const int DRIVE_SPEED = 255; 

// ===================================
// 2. BLE SETTINGS (Nordic UART Service)
// ===================================
#define BLE_SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define BLE_CHARACTERISTIC_RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" 
#define BLE_CHARACTERISTIC_TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" 

BLEServer* pServer = NULL;
BLECharacteristic* pRxCharacteristic = NULL;
bool deviceConnected = false;
char currentDriveDirection = 'S'; 

// ===================================
// 3. PROTOCOL DECODER & HELPERS
// ===================================
struct MicroBluePacket {
    std::string id = "";
    std::string value = "";
    bool valid = false;
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("BLE Connected.");
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      currentDriveDirection = 'S'; 
      BLEDevice::startAdvertising();
      Serial.println("BLE Disconnected. Advertising...");
    }
};

void stopMotors() {
    digitalWrite(MOTOR_IN1, LOW); digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW); digitalWrite(MOTOR_IN4, LOW);
    analogWrite(MOTOR_EN_A, 0);
    analogWrite(MOTOR_EN_B, 0);
}

void setMotorDirection(char direction, int speed) {
    if (direction == 'S') { stopMotors(); return; }
    speed = constrain(speed, 0, 255);

    if (direction == 'F') { // Forward
        digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
        digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
    } else if (direction == 'R') { // Reverse
        digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
        digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
    } else if (direction == 'L') { // Spin Left
        digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
        digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
    } else if (direction == 'G') { // Spin Right
        digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
        digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
    }
    analogWrite(MOTOR_EN_A, speed);
    analogWrite(MOTOR_EN_B, speed);
}

void handleSliderCommand(const std::string& id, const std::string& valueStr) {
    int angle = constrain(atoi(valueStr.c_str()), 0, 180);
    if (id == "sl0") { baseAngle = angle; baseServo.write(baseAngle); }
    else if (id == "sl1") { elbowAngle = angle; elbowServo.write(elbowAngle); }
    else if (id == "sl2") { wristAngle = angle; wristServo.write(wristAngle); }
}

void handleJawButton(bool isPressed) {
    if (isPressed) {
        int targetAngle = (jawAngle == JAW_CLOSE_ANGLE) ? JAW_OPEN_ANGLE : JAW_CLOSE_ANGLE;
        int currentAngle = jawServo.read();
        int step = (targetAngle > currentAngle) ? 1 : -1;
        for (int angle = currentAngle; angle != targetAngle; angle += step) {
            jawServo.write(angle);
            delay(10); 
        }
        jawServo.write(targetAngle);
        jawAngle = targetAngle;
    }
}

MicroBluePacket decodeMicroBlue(const std::string& data) {
    MicroBluePacket packet;
    if (data.length() < 3 || data[0] != 1) return packet; 
    size_t stx_pos = data.find(2);
    size_t etx_pos = data.find(3);
    if (stx_pos == std::string::npos || etx_pos == std::string::npos) return packet;
    packet.id = data.substr(1, stx_pos - 1);
    packet.value = data.substr(stx_pos + 1, etx_pos - stx_pos - 1);
    packet.valid = true;
    return packet;
}

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = std::string(pCharacteristic->getValue().c_str());
        MicroBluePacket packet = decodeMicroBlue(rxValue);
        if (!packet.valid) return;

        const std::string& id = packet.id;
        const std::string& val = packet.value;
        bool isPressed = (val == "1");

        if (id == "U") currentDriveDirection = isPressed ? 'F' : 'S';
        else if (id == "D") currentDriveDirection = isPressed ? 'R' : 'S';
        else if (id == "L") currentDriveDirection = isPressed ? 'L' : 'S';
        else if (id == "R") currentDriveDirection = isPressed ? 'G' : 'S';
        else if (id.find("sl") != std::string::npos) handleSliderCommand(id, val);
        else if (id == "b0") handleJawButton(isPressed);
    }
};

void setup() {
  Serial.begin(115200);

  // Servo Setup
  baseServo.attach(18, 500, 2400);  
  elbowServo.attach(16, 300, 2700); // Widened range for mechanical clearance
  wristServo.attach(17, 500, 2400); 
  jawServo.attach(23, 500, 2400);   

  // Initial Positions
  baseServo.write(baseAngle);
  elbowServo.write(elbowAngle);
  wristServo.write(wristAngle);
  jawServo.write(jawAngle); 

  // Motor Pin Setup
  pinMode(MOTOR_IN1, OUTPUT); pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT); pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_EN_A, OUTPUT); pinMode(MOTOR_EN_B, OUTPUT);
  stopMotors();

  // BLE Init
  BLEDevice::init("MicroBlue_RobotArm"); 
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
  pRxCharacteristic = pService->createCharacteristic(BLE_CHARACTERISTIC_RX_UUID, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEDevice::startAdvertising();
  Serial.println("Ready at Max Speed.");
}

void loop() {
    if (deviceConnected) setMotorDirection(currentDriveDirection, DRIVE_SPEED);
    else stopMotors();
    delay(10); 
}
