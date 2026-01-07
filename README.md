5-Axis Single Arm Mobile Manipulator
A high-precision 5-axis robotic arm integrated with an omnidirectional Mecanum wheel base. This project combines complex inverse kinematics with fluid mobile navigation for applications in automated sorting, research, and remote manipulation.

## Key Features
Omnidirectional Mobility: 4x Mecanum wheels allowing for lateral, diagonal, and rotational movement without changing orientation.

5-DOF Manipulation: Single arm featuring Base Rotation, Shoulder, Elbow, Wrist Pitch, and Wrist Roll.

Precision Control: Driven by [Insert Microcontroller, e.g., ESP32/Arduino/Raspberry Pi] using [Insert Stepper/Servo Drivers].

Kinematic Engine: Integrated Inverse Kinematics (IK) for the arm and velocity vector mapping for the Mecanum base.

## Hardware Architecture
1. The Base (Mecanum Drive)

The base uses four independent motors to achieve 3 degrees of freedom on a 2D plane.

Motors: [e.g., NEMA 17 Steppers or DC Geared Motors]

Wheels: [e.g., 60mm/97mm Mecanum Wheels]

Chassis: [e.g., Laser-cut Acrylic / 3D Printed / Aluminum Profile]

2. The Arm (5-Axis)

The arm is designed for a balance between reach and lifting capacity.

Axis	Function	Range	Actuator
J1	Base Rotation	±180°	[Insert Motor]
J2	Shoulder	-45° to 90°	[Insert Motor]
J3	Elbow	±150°	[Insert Motor]
J4	Wrist Pitch	±90°	[Insert Motor]
J5	Wrist Roll	Continuous	[Insert Motor]
## Kinematics & Mathematics
Mecanum Drive Logic

To translate desired velocity (V 
x
​	
 ,V 
y
​	
 ,ω) into individual wheel speeds, we use the following transformation:


​	
  
ω 
fl
​	
 
ω 
fr
​	
 
ω 
rl
​	
 
ω 
rr
​	
 
​	
  

​	
 = 
R
1
​	
  

​	
  
1
1
1
1
​	
  
−1
1
1
−1
​	
  
−(L+W)
(L+W)
−(L+W)
(L+W)
​	
  

​	
  

​	
  
V 
x
​	
 
V 
y
​	
 
ω
​	
  

​	
 
Arm Inverse Kinematics

The arm utilizes a geometric IK approach to calculate joint angles based on the (X,Y,Z) coordinates of the end-effector.

## Software Stack
Firmware: [e.g., C++/Arduino or MicroPython]

Communication: [e.g., ROS2, Serial, or Wi-Fi/UDP]

Libraries: * AccelStepper (for smooth motion)

TinyInverseKinematics (or custom IK scripts)

Installation

Clone the repo:

Bash
git clone https://github.com/yourusername/5-axis-mecanum-arm.git
Upload Firmware: Open src/main.cpp in VS Code (PlatformIO) or Arduino IDE and upload to your controller.

Configure Pins: Adjust config.h to match your specific motor driver wiring.

## Wiring Diagram
Note: Ensure you are using a dedicated power supply (e.g., 12V 10A LiPo) as the peak current for 9+ motors can exceed standard USB power.

## Usage
Power on the robot.

Connect via [Bluetooth/Serial/Web Interface].

Use the teleop script to control the base:

W/S: Forward/Backward

A/D: Strafe Left/Right

Q/E: Rotate

Input (X,Y,Z) coordinates for the arm via the CLI or GUI.

## Future Improvements
[ ] Add a 2-finger gripper with force feedback.

[ ] Integrate LiDAR for autonomous navigation (SLAM).

[ ] Implement MoveIt2 support for ROS2.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
