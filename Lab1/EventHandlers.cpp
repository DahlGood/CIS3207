#include "Simulator.h"
#include <queue>
#include <functional>

//Declaring all my queues and device status identifiers.
queue<Event> CPU_queue;
queue<Event> DISK1_queue;
queue<Event> DISK2_queue;
queue<Event> NETWORK_queue;
bool CPU_BUSY;
bool DISK1_BUSY;
bool DISK2_BUSY;
bool NETWORK_BUSY;

//Declaring new_time globally for conenience sake.
int new_time;

void handlePROCESS_ARRIVED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //prints the event to the log with no additional information.
    detailedLog(event, "");

    //Checking if the CPU is busy or currently has a non-empty queue.
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        //If the CPU is busy or the queue isnt empty:
        //Increment the amount of times CPUQ is used.
        setUsedCPUQ();
        //Print the event to the log, specifying that the event is being pushed to the queue.
        detailedLog(event, "CPU queue");
        //Put the event on the queue.
        CPU_queue.push(event);
    }
    //If the device is free and the queue is empty, give the process permission to go to the queue.
    else {
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }

    //Create a new process.
    event_queue.push(newEvent(PROCESS_ARRIVED_SYSTEM, event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX()), newProcess()));

}

void handlePROCESS_EXITED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    //Doing nothing. Process doesnt spawn any new events and it's just popped off the event queue.
}

void handlePROCESS_ARRIVED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    setUsedCPU();
    //Since a process arrived to the device, we say it's busy (busy processing).
    CPU_BUSY = true;
    //Creating a new time based on the most recent event time + a randomly generated number based on the min and max times it takes for a process to run in the CPU.
    //TLDR New time represents how long it takes the CPU to run.
    new_time = event.eventTime + getRandomBounds(getCPU_MIN(), getCPU_MAX());
    //Storing the amount of time it takes to run the process (response time).
    setTimeCPUQ(new_time - event.eventTime);
    //Storing the size of the device queue.
    setCPUQ(CPU_queue.size());

    //Telling the process to leave the device with it's finishing time.
    event_queue.push(newEvent(PROCESS_EXITED_CPU, new_time, event.eventProcess));

}

void handlePROCESS_EXITED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    
    detailedLog(event, "");
    //Since we're exiting the device, we say it's now no longer busy.
    CPU_BUSY = false;
    
    //Random chance the process will quit without accessing any IO device.
    if(getRandomBounds(1, 100) < getQUIT_PROB()) {
        event_queue.push(newEvent(PROCESS_EXITED_SYSTEM, event.eventTime, event.eventProcess));
    }
    //Random chance the process will use the device.
    else if(getRandomBounds(1, 100) < getNETWORK_PROB()) {
        if(NETWORK_queue.size() != 0 || NETWORK_BUSY) {
            setUsedNQ();
            detailedLog(event, "Network Queue");
            NETWORK_queue.push(event);
        }
        else {
            event_queue.push(newEvent(PROCESS_ARRIVED_NETWORK, event.eventTime + getRandomBounds(getNETWORK_MIN(), getNETWORK_MAX()), event.eventProcess));
        }
    }
    //If the processs didnt quit and didnt use the network then it's going to use one of the two disks.
    else {
        //If Disk1 is busy AND Disk1's Queue is smaller than Disk2's queue, then we put the process on the Disk1 Queue.
        if(DISK1_BUSY)  {
            if(DISK1_queue.size() < DISK2_queue.size()) {
                setUsedD1Q();
                detailedLog(event, "Disk 1 Queue");
                DISK1_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }
            //If Disk1 isnt busy we use the same logic to determine what disk queue the process will go on.
            else {
                setUsedD2Q();
                detailedLog(event, "Disk 2 Queue");
                DISK2_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }
            
        }
        //Same logic as before.
        else if(DISK2_BUSY) {
            if(DISK2_queue.size() < DISK1_queue.size()) {
                setUsedD2Q();
                detailedLog(event, "Disk 2 Queue");
                DISK2_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }
            else {
                setUsedD1Q();
                detailedLog(event, "Disk 1 Queue");
                DISK1_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }

        }
        //If none of the disks are busy, we randomly select which disk the process will go to. 50/50.
        else {
            if(getRandomBounds(1, 100) < 50) {
                 event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }
            else {
                event_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }

        }

    }

    //Now that the CPU is free, check to see if something is on the CPU Queue.
    if(CPU_queue.size() != 0) {
        //Handle process arrival to the CPU for the first element in the queue and say this occurs at the last eventTime + random CPU arrival time.
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, CPU_queue.front().eventProcess));
        //Now that we pushed the most recent queue item, remove it from the queue.
        CPU_queue.pop();
    }

}

void handlePROCESS_ARRIVED_DISK1(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    setUsedD1();
    DISK1_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getDISK1_MIN(), getDISK1_MAX());
    setTimeD1Q(new_time - event.eventTime);
    setD1Q(DISK1_queue.size());
    //Process leaves the disk after spending a randomly generated time on the disk.
    event_queue.push(newEvent(PROCESS_EXITED_DISK1, new_time, event.eventProcess));
}

void handlePROCESS_EXITED_DISK1(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    DISK1_BUSY = false;

    //If the CPU is busy or has a non-empty queue, put the process on the CPU queue.
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        setUsedCPUQ();
        detailedLog(event, "CPU Queue");
        CPU_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    //If the CPU isnt busy and has an empty queue put the process on the CPU.
    else {
        //Not adding time because the time the process spent on the disk was given by the calling function. Basically saying once a process is told to leave the disk, it doesnt take any meaningful amount of time to leave.
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    
    //Now that the Disk is free (because this is the exit disk handler obviously), check to see if something is on the Disk Queue.
    if(DISK1_queue.size() != 0) {
        //If a process was on the disk queue, put it on the disk it's waiting for.
        event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, DISK1_queue.front().eventProcess));
        //Since that process has now been sent to the disk it was waiting for, we remove it from the queue.
        DISK1_queue.pop();
    }

}

void handlePROCESS_ARRIVED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //Same logic as arrived disk 1.
    detailedLog(event, "");
    setUsedD2();
    DISK2_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getDISK2_MIN(), getDISK2_MAX());
    setTimeD2Q(new_time - event.eventTime);
    setD2Q(DISK2_queue.size());
    event_queue.push(newEvent(PROCESS_EXITED_DISK2, new_time, event.eventProcess));
}

void handlePROCESS_EXITED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //Same logic as exited disk 1.
    detailedLog(event, "");
    DISK2_BUSY = false;
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        setUsedCPUQ();
        detailedLog(event, "CPU Queue");
        CPU_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    else {
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }

    if(DISK2_queue.size() != 0) {
        event_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, DISK2_queue.front().eventProcess));
        DISK2_queue.pop();
    }

}

void handlePROCESS_ARRIVED_NETWORK(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //Same logic as the disks.
    detailedLog(event, "");
    setUsedN();
    NETWORK_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getNETWORK_MIN(), getNETWORK_MAX());
    setTimeNQ(new_time - event.eventTime);
    setNQ(NETWORK_queue.size());
    event_queue.push(newEvent(PROCESS_EXITED_NETWORK, new_time, event.eventProcess));
}

void handlePROCESS_EXITED_NETWORK(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //Same logic as the disks.
    detailedLog(event, "");
    NETWORK_BUSY = false;
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        setUsedCPUQ();
        detailedLog(event, "CPU Queue");
        CPU_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    else {
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }

    if(NETWORK_queue.size() != 0) {
        event_queue.push(newEvent(PROCESS_ARRIVED_NETWORK, event.eventTime, NETWORK_queue.front().eventProcess));
        NETWORK_queue.pop();
    }

}

void handleSIMULATION_FINISHED(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
        //When a simulation finished, do not continue processing anything else.
        //All we do is finish writing all the config and stats files.
        detailedLog(event, "");
        writeStats();

        //Exits the program.
        exit(0);
        
}
