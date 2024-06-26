# Hardware

## Brief

This directory contains the hardware design files for the TheftShield_Tracker project. The project utilizes the following key components:

**Microcontroller**

- STM32F401RCT6 - An ARM Cortex-M4 32-bit microcontroller from STMicroelectronics. It serves as the main control unit, handling task scheduling, communication with peripherals, and executing the application logic.

**GPS Module**

- NEO-6M - A high-performance GPS receiver module from u-blox. It provides accurate positioning data and enables tracking capabilities.

**GSM/GPRS Module**

- SIM800C - A quad-band GSM/GPRS module from SIMCOM. It enables SMS communication for remote control and data transmission.

**Other Components**

- MCP1700 - A low-dropout voltage regulator for providing a stable 3.3V supply.
- Backup battery holder and protection circuitry.
- Assorted passive components (resistors, capacitors, etc.).


## Design Files

- The KiCad project files containing the complete hardware design.
- [GPS tracker schematic](./TheftShield_Tracker.pdf) - A PDF version of the schematic.

## Block diagram

![GPS tracker hardware block diagram](../img/GPS%20tracker%20hardware.png)

