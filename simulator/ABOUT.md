A console application in C++ was developed to push data received through shared memory through Serial to an Arduino, which relays the data through a CAN Bus network. 

The Arduino replicates our cars CAN Bus network by transmitting all of the messages with the same ID's as the components on the car. This allows us to be able to test our software for various systems accurately through the Simulator.

To build the data stream code for the simulator, ensure that gcc is compiled on the machine. Also ensure that you have navigated to the correct working directory to access the make file.

``` >> make simulation 3 && ./simulation```

Once the console application is running ensure that the COM Port of the Arduino matches the target of the make file.