![WhatsApp Image 2026-01-07 at 11 46 50 AM](https://github.com/user-attachments/assets/6d9889ba-05a6-490a-a068-ad7ce029f494)
5-Axis Robot Arm with Mecanum Mobile Base (ESP32-BLE)
![WhatsApp Image 2026-01-07 at 11 48 53 AM](https://github.com/user-attachments/assets/79bf0e2e-aa61-4123-a637-df274bf5b22d)

A real-time, high-performance robotic system integrating a 5-Axis robotic arm and a high-speed mobile platform. The system is powered by an ESP32 and utilizes Bluetooth Low Energy (BLE) for low-latency control via the MicroBlue protocol.

🚀 5-Axis Kinematics

Base Rotation: 180-degree horizontal sweep for global orientation.

Elbow Actuation: Vertical reach with expanded PWM range for maximum clearance.

Wrist Pitch: Fine-tuned positioning of the end-effector.

Pivot Axis: Independent rotational axis for tool orientation (controlled via b1/b2).

Jaw/Gripper: Controlled clamping with gradual movement logic to ensure grip stability.

📦 Components List

Electronics

Microcontroller: 1x ESP32 DevKit V1 (30-pin).

Motor Driver: 1x L298N Dual H-Bridge Motor Driver Module.

Voltage Regulator: XL4015 or LM2596 Buck Converter.

Servos: 5x High-Torque Servos (Base, Elbow, Wrist, Pivot, Gripper).

Motors: 4x DC Geared Motors for Mecanum wheels.


<img width="1083" height="438" alt="image" src="https://github.com/user-attachments/assets/69a93b15-efce-4a63-bada-ebde233add68" />


⚙️ Mecanum Wheel & Motor Wiring

To control four Mecanum wheels using a single L298N driver, the motors are wired in two parallel banks (Left and Right).

L298N to ESP32 Connections

ENA (Speed Left): GPIO 33

IN1 / IN2 (Left Bank): GPIO 26 / GPIO 25

IN3 / IN4 (Right Bank): GPIO 19 / GPIO 5

ENB (Speed Right): GPIO 2

Motor to L298N Connections

OUT1 / OUT2: Connects to the Front-Left and Rear-Left motors in parallel.

OUT3 / OUT4: Connects to the Front-Right and Rear-Right motors in parallel.

Note: Ensure the polarity of the motors in each bank is matched so they rotate in the same direction for forward/reverse movement.

💻 Software & Protocol

Uses the MicroBlue Protocol: [SOH][ID][STX][Value][ETX]

Movement: 255 PWM (Maximum Speed).

Rest State: Base: 180°, Elbow: 90°, Pivot: 90°.

Developed as a part of an advanced robotics integration project focusing on embedded systems and wireless communication.

https://github.com/user-attachments/assets/e526a737-c255-4b1f-9925-bbf39ca0b8ec
