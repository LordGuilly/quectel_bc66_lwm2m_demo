# Quectel BC66 LWM2M demo
Code for testing the LWM2M AT command set of the BC66

## Dependencies
- A Quectel BC66 TE-B board, with NBIoT connectivity
- An MBED-OS compatible board, with Arduino standard headers
- MBED CLI
- GCC
- A Leshan server instance

## Introduction
This sample program connects to a Leshan server, using the Quectel BC66 AT command set.
It declares two objects in the client instace:
- A generic Digital Output, which follows the LED of the mbed board
- A generic Digital Input, which follows the User Button of the mbed board

These can be read and observed from the Leshan web interface, but not written (such 
operation will return "unauthorized"

It is currently tested with BC66 firmware version A07

## Setup
### Hardware
1. Make sure the BC66 TE-B board can register to a network, and the used APN parameters 
(using a USB cable and the TE-B board standalone, with a USB cable)
2. Connect the BC66 TE-B board to the MBED-OS compatible board
3. Check the UART switch of the BC66 board is in the MCU position

### Software
1. Check the parameters in the system.h file are correct:
   - the APN of the NBIoT provider
   - the NBIoT band
   - the MCU UART pins where the BC66 UART is connected
   - the PWRKEY and RESET pins
   - choose a unique device identifier name
2. Initialize an mbed program in the cloned repository
```
   mbed config root .
```
3. Configure the toolchain
```
   mbed toolchain GCC_ARM
```
4. Configure the Mbed board (the sample code was tested in a NUCLEO_F746ZG)
```
   mbed target NUCLEO_F746ZG
```
5. Fetch the MBED-OS code
```
   mbed deploy
```
6. Compile the code, and download it to the target
```
   mbed compile
```
