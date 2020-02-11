# Lab 1: Discrete Event Simulation
## Author: Luke Dependahl

### Program Workflow by Event
1. Process Arrived System
   1. If CPU is busy or if there are processes waiting for the CPU, push process to the CPU Queue
   2. Else put process on the CPU.
2. Process Arrived CPU
   1. Choose whether the device will quit, do network IO, or disk IO.
      1. If process quits, no longer process events for that process. Eventually it will be popped off of the event queue
      2. If process chooses network then check if the network is busy or if there are processes waiting for the network. If either of those are true, push the process to the event queue. Otherwise put the process on the network device.
      3. If the process chooses to do disk IO. Check if a disk is busy or if a process is waiting for it. If any of those is the case, put the process on the other device. If those cases are true for both devices, put the process on the device with the smallest queue.
      4. Create a new process (new process arrived system).
3. Process Arrived IO Device (Network, Disk1, Disk2)
   1. Exit the device after X time has elapsed.
   2. Request that the process be put back on the CPU (May be placed on the CPU queue or on the CPU).
   3. If there is a process in the devices queue, request that the process be put on the device.
![Basic-Disagram](general-workflow.png)
no woots