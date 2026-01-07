5-Axis Robot Arm with Mecanum Mobile Base (ESP32-BLE)

A real-time, high-performance robotic system integrating a 4-DOF (plus gripper) robotic arm and a Mecanum-wheeled mobile platform. The system is powered by an ESP32 and utilizes Bluetooth Low Energy (BLE) for low-latency control via the MicroBlue protocol.

🚀 Features

5-Axis Control: Includes Base rotation, Elbow, Wrist, and a precision Jaw/Gripper.

High-Speed Drivetrain: Mobile base configured for maximum performance (PWM duty cycle 255).

Low-Latency BLE: Implements the Nordic UART Service (NUS) for seamless smartphone integration.

Gradual Actuation: Smooth, degree-by-degree gripper movement to prevent servo jitter and mechanical stress.

Smart Initialization: Defined "Rest" positions (Base at 180°, Elbow at 90°) to ensure safe power-on sequences.

📦 Components List

Electronics

Microcontroller: 1x ESP32 DevKit V1 (30-pin).

Motor Driver: 1x L298N Dual H-Bridge Motor Driver Module.

Voltage Regulator: XL4015 or LM2596 Buck Converter (to step down battery voltage for servos/ESP32).

Servos: 4x MG996R or SG90 High-Torque Servos (Base, Elbow, Wrist, Gripper).

Motors: 4x DC Geared Motors (typically 3V-12V).

Battery: 7.4V (2S) or 11.1V (3S) Li-ion/LiPo battery pack.

Mechanical

Chassis: 4WD Aluminum or Acrylic Robot Chassis.

Wheels: 4x 60mm/80mm Mecanum Wheels (Omnidirectional).

Arm Structure: 3D Printed or Laser-cut Acrylic 4-DOF Robotic Arm Kit.

Hardware: M3 Screws, Nuts, and Brass Spacers.

🛠️ Hardware Requirements

Microcontroller: ESP32 (DevKit V1 or similar).

Motor Driver: L298N Dual H-Bridge Bridge.

Actuators: 4x DC Motors (Mecanum Wheel Configuration).

Servos: 4x High-Torque Servos (Base, Elbow, Wrist, Gripper).

Power Supply: 7.4V - 12V Li-ion or LiPo battery (with common ground to ESP32).

📋 Pin Mapping
<img width="1036" height="465" alt="Screenshot 2026-01-07 at 11 37 22 AM" src="https://github.com/user-attachments/assets/e0390de0-eb9f-448e-aa6a-d8e411258706" />

💻 Software & Protocol

The system uses a custom MicroBlue Packet Decoder to interpret serial data:
[SOH][ID][STX][Value][ETX]

Driving: Controlled via 'U' (Up), 'D' (Down), 'L' (Left), 'R' (Right).

Arm: Absolute positioning via sliders (sl0-sl2).

Gripper: Toggle logic via button b0.

🔧 Installation

Libraries: Install the ESP32Servo library via the Arduino Library Manager.

Board Manager: Ensure the ESP32 board package is installed (v3.0+ recommended).

Upload: Flash MicroBlue_Robot_Controller_Final.ino to your ESP32.

Connect: Open the MicroBlue app, scan for MicroBlue_RobotArm, and start controlling.

📝 Resume Highlights

Integrated 4-Axis Robotic System (ESP32/BLE): Developed and deployed C++ firmware on an ESP32 for the real-time control of a 4-axis robot arm and mobile base.

Advanced Control & Protocol Implementation: Established a robust BLE connection using NUS and implemented a custom serial protocol for simultaneous kinematic and motor control.

Developed as a part of an advanced robotics integration project focusing on embedded systems and wireless communication.
