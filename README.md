# UDS Bootloader

A C-based embedded project exploring how an AVR bootloader can receive firmware over UART and program it into flash.

## Project goal

The goal was to create a prototype of an ECU-style firmware-update flow: receive a programming request, transfer firmware data, write it to flash, and then start the application.

## Tools and technologies

- C
- AVR-GCC
- Eclipse
- USBasp
- UART
- AVR flash-programming and EEPROM interfaces

## What I implemented

- Organized the code into application startup, UART communication, and flashing-management modules.
- Created a state-machine structure for a programming session and firmware transfer.
- Added handlers for UDS-related service IDs `0x10`, `0x34`, `0x36`, `0x37`, and `0x31`.
- Added AVR flash-page programming and EEPROM-based boot/application selection.
- Included a CRC-related handler; CRC verification is not yet complete.

## Service IDs in this prototype

| ID | Purpose |
|---|---|
| `0x10` | Select a programming session |
| `0x34` | Request a firmware download |
| `0x36` | Transfer firmware data |
| `0x37` | End the data transfer |
| `0x31` | CRC-related routine (not developed yet) |

## How the project works

1. Startup logic checks EEPROM flags to select the bootloader or application path.
2. The UART module receives a request and passes it to the flashing manager.
3. The flashing manager processes the programming session, download, data transfer, and transfer exit.
4. AVR boot routines write received data to flash pages.
