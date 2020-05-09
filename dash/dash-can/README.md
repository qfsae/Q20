# Dash CAN Bus Communication

Due to an issue where the CAN controller and screen do not function while
sharing the same SPI bus, the Dash has been broken up to use two
Microcontrollers. An ATmega328P will be used to send and receive CAN messages on
behalf of the dash. The ATmega328P is the same MCU found on a standard Arduino
Uno. The Teensy is designated as the display driver while the ATmega328P is the
CAN controller. The two MCUs are linked together via UART (Universal
Asynchronous Serial Receiver Transmitter). UART was chosen because Arduino
provides a very simple interface for reading and write bytes on the bus in the
functions `Serial.read()` and `Serial.write()`. Moreover, the Teensy has
multiple UARTs on board so communication with the CAN controller can be done
while still printing debug related data to the Serial monitor on the computer.

# Software Implementation

A rough software implementation the communication over UART can be found in the
`can-contrller` and `screen-basic` sketches. This code will be formalized into a
C++ Arduino library that provides method for sending and receiving the data over
UART.

## TODO: Interface

Specify stub methods here.

Read method will return some type of `DashData` structure with fields for each
piece of data the screen must show.  

The `DashData` structure will contain the following fields:
- RPM (int)
- TPS (int)
- Pressure unit (unsigned char)
- Fuel Pressure (int)
- Battery Voltage (int)
- Oil Pressure (int)
- Engine Temp (int) 

Write method will take any even number of bytes and add the unique message end
marker which contains a value that cannot be transmitted by the ECU.
