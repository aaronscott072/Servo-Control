# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.0] - 2022-09-12
### Added
- Operational mode management backbone and related status LEDs.
    - Operational modes / LED status indications: 
        - Unknown (during MCU start-up) / None
        - Idle / Green
        - Motor running / Yellow
        - Error state: firmware fault / Red & Yellow
        - (to be implemented) Error state: motor / Red
        - (to be implemented) Error state: LCD / Red
        - (to be implemented) Error state: peripherals (USART, etc.) / Red
- Documents: STM32L4 datasheet/ref-manual, NUCLEO schematics/pinouts, and HD44780U datasheet and LCD pinout.
- FreeRTOS wrapper driver:
    - Provides a simpler FreeRTOS interface by abstracting API features.
- Servo motor control initial features:
    - PWM control signal (20 ms period)
    - Ability to set position (angle in degrees) of the motor shaft by setting the PWM pulse-width in range (0.5..2.5) ms to move the position in range (-90..+90) degrees.
- LCD base driver features:
    - Display initialisation
    - Ability to clear and write lines to the display.
- This version contains these key features:
    - The servo motor shaft position (angle in degrees) oscillates between (-90..+90) degrees.
    - The position and operational mode are displayed on the LCD.
    - The position and operational mode are transmitted to the virtual COM port.
    - LEDs indicate when the operational mode is "idle" (green), "motor running" (yellow), or "error state: firmware fault" (red & yellow).

## [0.1.0] - 2022-09-09
### Added
- Project backbone containing:
    - Makefile build setup and file structure,
    - STM32L433 HAL base,
    - FreeRTOS core,
    - USART 2 configured to enable the NUCLEO-L433RC-P dev board's USB connection to act as a virtual COM port (for debugging, motor status/control, UI, etc.),
    - relevant files (README, CHANGELOG, openocd.cfg, .gdbinit, an .ioc file), and
    - relevant third-party drivers (STM32L433 HAL, FreeRTOS, CMSIS).