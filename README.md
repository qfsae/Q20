<<<<<<< HEAD
# Q20
=======
# Queen's Formula SAE Team - Q20

This repository was used throughout the 2019-2020 year to maintain code, documentation and develop the Q20 Electrical and Software systems. 

This repository has been moved to a GitHub group, which can be found [here](https://github.com/qfsae)

## Projects

### Dash Display
The Q20 dashboard makes use
of a high brightness and high definition touch display for visibility under all
lighting conditions. The display provides
the driver with their RPM, gear and speed as well as a diagnostic data screen
containing battery voltage and other debugging information. The display is
driven by a Teensy 3.2 microcontroller over SPI. All the information displayed
on the screen is obtained from the car's ECU and PDM which send the data over
CAN bus. The CAN bus data is received and filtered by a dedicated ATmega328P
microcontroller which then sends the relevant CAN messages to the Teensy driving
the display via UART. Links to both the CAN controller and Teensy display code
are given below.

- [Teensy Display](https://github.com/bchampp/Q20/tree/master/dash/dash-code-final)
- [CAN Controller](https://github.com/bchampp/Q20/tree/master/dash/dash-can/can-controller)

### Electronic Throttle Control
A custom Electronic Throttle Control System was developed for the Q20 vehicle featuring an Arduino Mega, a high current motor driver and the Bosch 32mm Electronic Throttle Body. A PID control system is used to drive a feedback loop between the Throttle Pedal and the Throttle Body. Hardware interrupts are configured to blip the throttle during shifting sequences. The code for the Electronic Throttle control system and a custom test bench are given below. 
- [Control System](https://github.com/bchampp/Q20/tree/master/electronic-throttle/control-system)
- [Test Bench](https://github.com/bchampp/Q20/tree/master/electronic-throttle/testbench)
### Simulator
The team has developed a pipeline to simulate our CAN communication protocols
through the game Assetto Corsa. A custom C++ program is used to monitor the
shared memory from Assetto Corsa, contains the RPM, speed and other diagnostic
information of the car being driven in the game. The contents of the shared
memory are sent to a CAN enabled Arduino Uno over USB. The Arduino Uno uses the
racing simulator data to send out CAN bus messages mimicking the car's ECU and
PDM to provide a full testbench for the car's electrical system with a realistic
stream of testing data provided by Assetto Corsa. The code for both the C++
program and the CAN enabled Arduino are given below.

- [Assetto Corsa Interfacing Program](https://github.com/bchampp/Q20/blob/master/simulator/sim-data/DataStream.cpp)
- [CAN Bus Arduino Code](https://github.com/bchampp/Q20/blob/master/simulator/sim-data/sim-data-arduino-v2/sim-data-arduino-v2.ino)

## Authors
- [Brent Champion](https://github.com/bchampp)
- [Logan Lim](https://github.com/llim1)
- [Ethan Peterson](https://github.com/ethanmpeterson)
- [Ryan Anderson](https://github.com/ryan-and)
- [Jacob O'Riordan Ross](https://github.com/JacobORiordanRoss)
- [Angus Hodgins](https://github.com/angushodgins)
- [Nick Totman](https://github.com/necm77)
- [Alex Koch](https://github.com/alexkoch14)

## License
All of the projects in this repository are licensed under the MIT License - see the [LICENSE](https://github.com/Q20/blob/master/LICENSE) file for details
>>>>>>> oldrepo/master
