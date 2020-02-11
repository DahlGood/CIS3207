# CIS3207 Labs

## Lab0

wcat
wgrep
wzip
wunzip

## Lab1 - Discrete Event Simulation
###Giorgio's Discrete Event Simulator
####Program to simulate the most basic relationships and machanisms of the CPU, Disks, and Network.

######Program Flow
*Process arrives to the system.
*Process is sent to the CPU to be processed.
*Process arrives to the CPU.
**CPU determines if the process will end or use an IO device.
*If the process chooses to go to an IO device, we check if the device is used.
**If it's being used we put it in a queue till the device is ready.
**Otherwise sit on the device for X amount of time then go back to the CPU.