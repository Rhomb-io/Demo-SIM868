# rhomb.io SIM868 Slave Module demo

This demo shows how to use the rhomb.io SIM868 module using the PlatformIO IDE with the Arduino framework.

The [rhio-pinmap](https://github.com/Rhomb-io/rhio-pinmap) library is included to handle pin names with any AVR or SAMD21 MCU architecture.

## Power Management

An important part of the SIM868 slave module is power management. By default the modem is turned off, it does not receive current. It is necessary to enable the PWREN and PWRKEY lines in order to use it. This energy control will be highly appreciated by developers working on low power applications.

# Serial Ports

Two UART ports are used, one for debug and one for sending AT commands to the modem. Configuration is included for Arduino One, Mega and Zero.

## AT Commands

This demo will simply turn on the modem and send a couple of AT commands. It is recommended to use a library to parse AT commands (not included). Other libraries such as [TrackioUno](https://github.com/M2MSystemSource/TrackioUno) or [TinyGSM](https://github.com/vshymanskyy/TinyGSM) may be useful.

## Arduino IDE

To use this demo in the Arduino IDE you can create a new sketch and copy the contents of the src/main.cpp file. You will also need to use the arduino library manager to install the [rhio-pinmap](https://github.com/Rhomb-io/rhio-pinmap) library or download and install it manually.

## Support

Please use the issues section if you need help, we will be happy to help you.
