# CIS3207 Labs

## Lab0

wcat
wgrep
wzip
wunzip

## Lab1 - Discrete Event Simulation
### Giorgio's Discrete Event Simulator
#### Program to simulate the most basic relationships and machanisms of the CPU, Disks, and Network.

##### Program Flow:
1. Process arrives to the system.
1. Process is sent to the CPU to be processed.
1. Process arrives to the CPU.
    1. CPU determines if the process will end or use an IO device.
1. If the process chooses to go to an IO device, we check if the device is used.
    1. If it's being used we put it in a queue till the device is ready.
    1. Otherwise sit on the device for X amount of time then go back to the CPU.