# Queen's Formula SAE Team - Q20

Codebase and configurations for QFSAE Q20 team. The team is currently using the MoTeC M150 ECU, PDM15 and L180. 

This repository is used to maintain all of the code regarding CAN Bus protocol, the Q20 dash system, custom PCB projects and documentation for all other components of the electrical system. 

This repository will also be used as an archive of various versions of each system.

## Folder Directory
- CAN Bus
- dash
- data
- ecu
- harness
- mcu
- pdm
- schematics
- shifting
- sim-data
- testing

## Dash Display

## Harness

## Simulator
The team has developed a pipeline to simulate our CAN communication protocols through the game Assetto Corsa. 

A console application in C++ was developed to push data received through shared memory through Serial to an Arduino, which relays the data through a CAN Bus network. 

The Arduino replicates our cars CAN Bus network by transmitting all of the messages with the same ID's as the components on the car. This allows us to be able to test our software for various systems accurately through the Simulator.

To build the data stream code for the simulator, ensure that gcc is compiled on the machine. Also ensure that you have navigated to the correct working directory to access the make file.

``` >> make simulation 3 && ./simulation```

Once the console application is running ensure that the COM Port of the Arduino matches the target of the make file.

