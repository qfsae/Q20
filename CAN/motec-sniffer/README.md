# Notes About CAN on Motec M150 ECU
- "Channel" notation corresponds to the bytes at a given CAN address
  * Ex. Channel 0 would correspond to the contents of index 0 in the CAN packet buffer
- Screenshots containing the addressing info found in docs folder

# Todo

- Test for stability at a higher bit rate on the CAN bus
  * Currently running at 250KBPS, aiming for 500KBPS or 1MBPS
  * Motec Tune software produces warning for transmit overrun as our bit rate
    is not fast enough to handle the frequency at which we have configured
    certain messages to send at.
  * Further optimization to increase performance is removing all Serial print
    statements in the production code as prints take a long time as they are not
    implemented asynchronously in the Arduino standard library thus the
    functions dont exit until the bytes have been loaded into the UART send queue.
- Address sensor faults for both fuel and coolant temp
  * coolant currently reads 80 C (engine off)
  * fuel reads (30 C) where as oil reads a more reasonable 21 C
- Look into possible CAN configuration error with receive on the ECU from PDM
  (can still observe the messages being sent from PDM on Arduino but not through
  M1 Tune?)

# Things to get from the data logger
- Anything and everything from the PDM
- Look into into session reset mechanism

