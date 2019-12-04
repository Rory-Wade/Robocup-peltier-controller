# PELTIER CONTROL UNITS (Australian Units)

## 1.	POWER
The units are powered through a 2.5mm DC jack with a positive center. Input tolerance is 10V – 14V 5A. 
### A.	Powering On
i.	On unit power up the device should blink a white (or blue on V1) LED and have a solid blue LED.
ii.	A dim blue LED or a non-flashing white LED is an indication of not having appropriate amount of power. 
### B.	Reset
i.	The device can be reset by pushing a small object into the reset hole on the case. The device should flask the LED once the reset is complete.

## 2.	CONNECTIONS
The device has 5 independent Peltier control ports comprised of a screw terminal and 2 pin plug connector. The Peltier wires should be connected such that the warm side of the Peltier is facing the thermistor. The thermistor should also be plugged into the appropriate 2 pin connector. 

## 3.	OPERATION
Each Peltier port is controlled via a the a dip switch. Although the devices can power up to 5 peltries, only 4 should be used at a single time. This allows for “Dummy” victims to be setup. Care should be taken to monitor the flashing LED. In the event that it in solid on or off the device should be reset.

**NOTE: the device allows for 5 Peltiers to be running but will make the device unstable and prone to browning out in an on state. 4 peltries should be on at a single time.**

## 4.	MODIFYING TEMPERATURES
A serial head is provided with on the PCB to interface with the devices at a board rate of 9600. The header is a standard interface of the DFR0164. The protocol can be discovered by sending the character “?”. The options give the ability to modify select and global offsets for all Peltier devices. Allowing for the used to add, subtract and set the temperatures.


