# Microwave Oven Controller

Embedded C project for a microwave oven controller, developed for a PIC microcontroller using MPLAB X IDE and the XC8 compiler.

## Features
- Startup screen and operating-mode menu
- Micro, Grill, and Convection modes
- User-configurable cooking time through a keypad
- Preheating sequence for Grill and Convection modes
- Start, pause, resume, and stop controls
- LCD status display
- Buzzer, fan, and heater output control

## Hardware and software
- PIC microcontroller (confirm the exact device used in the MPLAB project configuration)
- Character LCD (CLCD)
- Matrix keypad
- Buzzer, fan, and heater driver/interface circuits
- MPLAB X IDE
- MPLAB XC8 compiler

## Source layout
- `main.c` – application state machine and main control flow
- `microwave_def.h` – pin definitions, limits, and state/mode declarations
- `clcd.c`, `clcd.h` – character LCD interface
- `keypad.c`, `keypad.h` – keypad interface
- `isr.c` – interrupt service routine

## Build
1. Open the `.X` project folder in MPLAB X IDE.
2. Select the matching PIC device and XC8 toolchain configured for the project.
3. Build the project.
4. Connect the required driver circuits before testing physical outputs.

## Notes
This repository contains the project source and MPLAB project configuration. Hardware wiring, simulation screenshots, and a verified device-specific bill of materials can be added when available.

**Safety:** Do not connect a real microwave oven's high-voltage or mains-powered heater circuitry directly to a microcontroller. Use properly rated, isolated driver circuitry and qualified supervision.
