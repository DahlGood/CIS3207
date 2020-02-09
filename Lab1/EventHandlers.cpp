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

int system_time;

void handlePROCESS_ARRIVED_SYSTEM(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    system_time = event.eventTime;
    //May be a problem with data not being updated correctly. Maybe play with pointers to avoid having to return :)
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        CPU_queue.push(event);
    }
    else {
        event_queue.push(newEventB(PROCESS_ARRIVED_CPU));
    }

    //cout << "Old Event Type = " << event.eventType << endl;
    //cout << "Old Event Time = " << event.eventTime << endl;
    //cout << "Old Event PID = " << event.eventProcess << endl;
    //Event new_event = newEventB(PROCESS_ARRIVED_CPU); // this might be a problemmmmm the newEventB function.
    
    //cout << "New Event Type = " << new_event.eventType << endl;
    //cout << "New Event Time = " << new_event.eventTime << endl;
    //cout << "New Event PID = " << new_event.eventProcess << endl;
    
    event_queue.push(newEvent(PROCESS_ARRIVED_SYSTEM, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , newProcess()));
    /*
        If CPU is occupied or queue is not empty
            put process on CPU queue
        Elso
            create new event process arrive cpu
                event time = time from old event.
                set CPU to occupied.
        Create new event for process arrival
            event time = current system time + random interval
            create new process with a unique ID
    */
}

void handlePROCESS_EXITED_SYSTEM(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    //event_queue.pop();
}

void handlePROCESS_ARRIVED_CPU(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    /*
    create new event PROCESS_FINISH_CPU
    event time = new time
    */
    CPU_BUSY = true;
    event_queue.push(newEvent(PROCESS_EXITED_CPU, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));

}

void handlePROCESS_EXITED_CPU(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    CPU_BUSY = false;
    double randomProb = getRandomBounds(1, 100);
    
    if(randomProb < getQUIT_PROB()) {
        cout << "quit\n";
        event_queue.push(newEventB(PROCESS_EXITED_SYSTEM));
    }
    else if(randomProb < getNETWORK_PROB()) {
        cout << "network\n";
        if(NETWORK_queue.size() != 0 || NETWORK_BUSY) {
            cout << "network queue\n";
            NETWORK_queue.push(event);
        }
        else {
            cout << "network push\n";
            event_queue.push(newEvent(PROCESS_ARRIVED_NETWORK, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
        }
    }
    else {
        cout << "disk1\n";
        if(DISK1_queue.size() < DISK2_queue.size() || DISK2_BUSY) {
            if(DISK1_BUSY) {
                cout << "disk1 queue\n";
                DISK1_queue.push(event);
            }
            else {
                cout << "disk1 push\n";
                event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
            }
        }
        else {
            if(DISK2_BUSY) {
                cout << "disk2 queue\n";
                DISK2_queue.push(event);
            }
            else {
                cout << "disk2 push\n";
                event_queue.push(newEvent(PROCESS_ARRIVED_DISK2, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
            }
        }
        
    }

    if(CPU_queue.size() != 0) {
        CPU_queue.pop();
        event = CPU_queue.front();
        event_queue.push(newEvent(PROCESS_ARRIVED_CPU, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));

    }

    /*
    if random(random double between 0 and 1 < QUIT_PROB) {
        create new event, PROCESS EXIT
        assign process from old event to new __eventassign new event time to old event time aka process exit cpu happens at the same time as process exit system.
        eventque.push
    }

    if it doesnt exit, decide what queue it goes into. DISK OR NETWORK
    generate random number and decide if it goes to network.
    If not network. Check if disk is occupied, if they both are choose the one with the smallest queue.
        create new event disk process arrived on disk.
    */
    //In each process handle, you create the next ptocess
    //The exit events have the responsibility of checking the queue.
    //If the queue is not empty, create a new event process arrived on CPU
}

void handlePROCESS_ARRIVED_DISK1(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    DISK1_BUSY = true;
    event_queue.push(newEvent(PROCESS_EXITED_DISK1, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventType));
}

void handlePROCESS_EXITED_DISK1(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    /*
    Set disk to not occupied
    IF cpu occupied 
        pur process on the cpu queue
    */
    DISK1_BUSY = false;
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        CPU_queue.push(event);
    }
    event_queue.push(newEventB(PROCESS_ARRIVED_CPU));

    if(DISK1_queue.size() != 0) {
        DISK1_queue.pop();
        event_queue.push(newEvent(PROCESS_ARRIVED_DISK1, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
    }

}

void handlePROCESS_ARRIVED_DISK2(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    DISK2_BUSY = true;
    event_queue.push(newEvent(PROCESS_EXITED_DISK2, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
}

void handlePROCESS_EXITED_DISK2(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {

    DISK2_BUSY = false;
    if(CPU_queue.size() != 0 || CPU_BUSY) {
        CPU_queue.push(event);
    }
    event_queue.push(newEventB(PROCESS_ARRIVED_CPU));

    if(DISK2_queue.size() != 0) {
        DISK2_queue.pop();
        event_queue.push(newEvent(PROCESS_ARRIVED_DISK2, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
    }
}

void handlePROCESS_ARRIVED_NETWORK(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    NETWORK_BUSY = true;
    event_queue.push(newEvent(PROCESS_EXITED_NETWORK, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
}

void handlePROCESS_EXITED_NETWORK(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
    NETWORK_BUSY = false;
    if(NETWORK_queue.size() != 0 || NETWORK_BUSY) {
        CPU_queue.push(event);
    }
    event_queue.push(newEventB(PROCESS_ARRIVED_CPU));

    if(DISK1_queue.size() != 0) {
        DISK1_queue.pop();
        event_queue.push(newEvent(PROCESS_ARRIVED_NETWORK, (event.eventTime + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX())) , event.eventProcess));
    }
}

void handleSIMULATION_FINISHED(Event event, priority_queue<Event, vector<Event>, EventComparator> &event_queue) {
        
        //Finish writing to log file.
        //Write statistics to STAT file
        //Close all file handles
        exit(1);
        

}
