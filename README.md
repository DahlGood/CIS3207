# CIS3207 Labs

## [Lab0 - Utilities](Lab0/)
### Linux Utilities
#### Simple recreation of linux utilies
cat, grep, zip, unzip

## [Lab1 - Discrete Event Simulation](Lab1/)
### Giorgio's Discrete Event Simulator
#### Program to simulate the most basic relationships of devices
Simulated CPU, Disks, and Network.

## [Lab2 - Shell](Lab2/)
###  Based on Ritchie and Thompson's UNIX Shell Implementation
#### Simple Shell implementation that handles bash commands
Handles bash commands <, >, >>, |, & on various built-in funtions and all external programs present on the system.

## [Lab3 - Spell Checker](Lab3/)
### Network Spell Checker
#### Multi-Threaded Network Spell Checker.

The project is broken into two parts. The server and the client. The server accepts connections and the client connects. Once a connection is made the client can send the server a word. The server spell checks that word and tells the client whether it was correctly spelled or not. These are two seperate applications and do not depend on each other at all, however the client was developed with our server in mind.

## [Lab4 - Signaling](Lab4/)
### Interprocess / Interthread Communication
#### Signaling With Multi-Process & Multi-Threaded Programs 

This lab demonstrates the use of signals for inter-process and inter-thread communication. These two types of communication are implemented in two different programs both with the goal of completing the same task. Both programs create a thread or processes respectively. Each thread has a main function, a reporting function, three signal counting functions, and three signal generating functions.