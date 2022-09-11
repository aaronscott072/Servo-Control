# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Operational mode management backbone and related status LEDs.
    - Operational modes / LED status indications: 
        - Unknown (during MCU start-up) / None
        - Idle / Green
        - Motor running / Yellow
        - Error state: motor / Red
        - Error state: LCD / Red
        - Error state: peripherals (USART, etc.) / Red
        - Error state: firmware fault / Red & Yellow
- Documents: STM32L4 datasheet/ref-manual and NUCLEO schematics/pinouts.
- FreeRTOS wrapper driver:
    - Provides a simpler FreeRTOS interface by abstracting API features.

## [0.1.0] - 2022-09-09
### Added
- Project backbone containing:
    - Makefile build setup and file structure,
    - STM32L433 HAL base,
    - FreeRTOS core,
    - USART 2 configured to enable the NUCLEO-L433RC-P dev board's USB connection to act as a virtual COM port (for debugging, motor status/control, UI, etc.),
    - relevant files (README, CHANGELOG, openocd.cfg, .gdbinit, an .ioc file), and
    - relevant third-party drivers (STM32L433 HAL, FreeRTOS, CMSIS).