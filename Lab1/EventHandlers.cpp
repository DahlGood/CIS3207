#include "Simulator.h"
#include <queue>
#include <functional>

queue<Event> CPU_queue;
queue<Event> DISK1_queue;
queue<Event> DISK2_queue;
queue<Event> NETWORK_queue;
bool CPU_BUSY;
bool DISK1_BUSY;
bool DISK2_BUSY;
bool NETWORK_BUSY;

int new_time;


void handlePROCESS_ARRIVED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        setUsedCPUQ();
        detailedLog(event, "CPU queue");
        CPU_queue.push(event);
    }
    else {
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    event_queue.push(newEvent(PROCESS_ARRIVED_SYSTEM, event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX()), newProcess()));

}

void handlePROCESS_EXITED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    //Doing nothing. Process doesnt spawn any new events and it's just popped off the event queue.
}

void handlePROCESS_ARRIVED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    CPU_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getCPU_MIN(), getCPU_MAX());
    setTimeCPUQ(new_time - event.eventTime);
    setCPUQ(CPU_queue.size());
    event_queue.push(newEvent(PROCESS_EXITED_CPU, new_time, event.eventProcess));

}

void handlePROCESS_EXITED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    
    detailedLog(event, "");
    CPU_BUSY = false;

    //double randomProb = getRandomBounds(1, 100);
    
    if(getRandomBounds(1, 100) < getQUIT_PROB()) {
        event_queue.push(newEvent(PROCESS_EXITED_SYSTEM, event.eventTime, event.eventProcess));
    }
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
    else {
        cout << "here" << endl;
        if(DISK1_BUSY)  {
            cout << "DISK1_BUSY" << endl;
            if(DISK1_queue.size() < DISK2_queue.size()) {
                cout << "Disk 1 Queue Used" << endl;
                setUsedD1Q();
                detailedLog(event, "Disk 1 Queue");
                DISK1_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }
            else {
                cout << "Disk 2 Queue Used" << endl;
                setUsedD2Q();
                detailedLog(event, "Disk 2 Queue");
                DISK2_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }
            
        }
        else if(DISK2_BUSY) {
            cout << "DISK2_BUSY" << endl;
            if(DISK2_queue.size() < DISK1_queue.size()) {
                cout << "Disk 2 Queue Used" << endl;
                setUsedD2Q();
                detailedLog(event, "Disk 2 Queue");
                DISK2_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }
            else {
                cout << "Disk 1 Queue Used" << endl;
                setUsedD1Q();
                detailedLog(event, "Disk 1 Queue");
                DISK1_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }

        }
        else {
            
            if(getRandomBounds(1, 100) < 50) {
                 event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));
            }
            else {
                event_queue.push(newEvent(PROCESS_ARRIVED_DISK2, event.eventTime, event.eventProcess));
            }
            
            //event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, event.eventProcess));

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
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        setUsedCPUQ();
        detailedLog(event, "CPU Queue");
        CPU_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    else {
        //Not adding time because the time the process spent on the disk was given by the calling function. Basically saying once a process is told to leave the disk, it doesnt take any meaningful amount of time to leave.
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, event.eventTime, event.eventProcess));
    }
    
    //Now that the Disk is free, check to see if something is on the CPU Queue.
    if(DISK1_queue.size() != 0) {
        event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, event.eventTime, DISK1_queue.front().eventProcess));
        DISK1_queue.pop();
    }

}

void handlePROCESS_ARRIVED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    detailedLog(event, "");
    DISK2_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getDISK2_MIN(), getDISK2_MAX());
    setTimeD2Q(new_time - event.eventTime);
    setD2Q(DISK2_queue.size());
    event_queue.push(newEvent(PROCESS_EXITED_DISK2, new_time, event.eventProcess));
}

void handlePROCESS_EXITED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
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
    detailedLog(event, "");
    NETWORK_BUSY = true;
    new_time = event.eventTime + getRandomBounds(getNETWORK_MIN(), getNETWORK_MAX());
    setTimeNQ(new_time - event.eventTime);
    setNQ(NETWORK_queue.size());
    event_queue.push(newEvent(PROCESS_EXITED_NETWORK, new_time, event.eventProcess));
}

void handlePROCESS_EXITED_NETWORK(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
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
        detailedLog(event, "");
        writeStats();
        //Finish writing to log file.
        //Write statistics to STAT file
        //Close all file handles

        exit(1);
        

}
