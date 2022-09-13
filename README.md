# Servo Motor Control Project

## Table of Contents
1. [Project Purpose](#ProjectPurpose)
2. [Hardware Configuration](#HardwareConfiguration)
3. [Features](#Features)
    - 3.1. [Version 0.2.0](#Version0.2.0)
    - 3.2. [ToDo](#ToDo)
4. [Project Photos](#ProjectPhotos)
    - 4.1. [Photos from Version 0.2.0](#PhotosFromVersion0.2.0)
    
## Project Purpose <a name="ProjectPurpose"></a>
This project serves these purposes:
1. To revise control concepts (i.e. PID) and practice implementing them on an MCU.
2. To act as a showcase/demonstration of my current coding style/choices.

## Hardware Configuration <a name="HardwareConfiguration"></a>

### Hardware Diagram<a name="HardwareDiagram"></a>

```mermaid
graph TD
subgraph Development Board: NUCLEO-L433RC-P
    A(MCU<br/>STM32L433RCT6P)
    B(Virtual COM Port)
    C(ST-LINK<br/>Debugger/Programmer)
end
B --- D[Dev Computer]
C --- D
A -- PWM --> E(Servo Motor)
A -- GPIO x6 --> F(LCD)
A -- GPIO x3 --> G(LEDS)
A -- USART2 --- B
```
## Features <a name="Features"></a>

### Version 0.2.0 <a name="Version0.2.0"></a>
- This version contains these key features:
    - The servo motor shaft position (angle in degrees) oscillates between (-90..+90) degrees.
    - The position and operational mode are displayed on the LCD.
    - The position and operational mode are transmitted to the virtual COM port.
    - LEDs indicate when the operational mode is "idle" (green), "motor running" (yellow), or "error state: firmware fault" (red & yellow).

### ToDo <a name="ToDo"></a>

- **Motor Controller**
    - Close the loop to receive feedback from the servo motor by soldering a connection to the motor's potentiometer wiper
- **User Interface**
	- Add the ability for a user to input motor angles and other commands via the LCD (add pushbuttons to the hardware setup) and the virtual COM port
	- Add status/telemetry that is broadcast to the LCD and virtual COM port such as actual vs. expected angle, velocity, acceleration, etc.
- **Firmware**
	- Convert servo/LCD/LED drivers to object-oriented and generic, move them to a common repository so that I can use them across multiple projects with a varying number of those devices.

## Project Photos <a name="ProjectPhotos"></a>

### Photos from Version 0.2.0 <a name="PhotosFromVersion0.2.0"></a>

![Hardware Top View (System is Idle)](docs/PROJECT-PHOTOS/v0.2.0-HW-TopView-Idle.jpg?raw=true "v0.2.0-HW-TopView-Idle")
![Hardware Top View (Motor is Running)](docs/PROJECT-PHOTOS/v0.2.0-HW-TopView-MotorRunning.jpg?raw=true "v0.2.0-HW-TopView-MotorRunning")
![Virtual COM Port (Motor is Running)](docs/PROJECT-PHOTOS/v0.2.0-SW-VirtualCOMPort-MotorRunning.PNG?raw=true "v0.2.0-SW-VirtualCOMPort-MotorRunning")