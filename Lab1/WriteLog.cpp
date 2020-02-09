#include <fstream>
#include <string.h>
#include "Simulator.h"

string processName[11];

void detailedLog(Event event, string action) {
    //Opening a file.
    ofstream io;
    //Making sure file was opened correctly.
    io.open("detailed_log.txt", ios::app);
    if(!io.is_open()) {
        cout << "Didn't open file correctly.";
        exit(EXIT_FAILURE);
    }

    if(action == "") {
        io << "At " << event.eventTime << " Process " << event.eventProcess << " " << processName[event.eventType] << endl;
    }
    else {
        io << "At " << event.eventTime << " Process " << event.eventProcess << " Pushed to " << action << endl;
    }

    io.close();
}

void printEvent(Event event) {
    cout << "At " << event.eventTime << " PID: " << event.eventProcess << " " << processName[event.eventType] << endl;
}

void printQueue(priority_queue<Event, vector<Event>, EventComparator> event_queue) {
    int size = event_queue.size();
    for(int i = 0; i < size; i++) {
        printEvent(event_queue.top());
        event_queue.pop();
    }
}

void clearLog() {
    remove("log.txt");
    remove("detailed_log.txt");

}

void setProcessNames() {
    processName[0] = "PROCESS_ARRIVED_SYSTEM";
    processName[1] = "PROCESS_EXITED_SYSTEM";
    processName[2] = "PROCESS_ARRIVED_CPU";
    processName[3] = "PROCESS_EXITED_CPU";
    processName[4] = "PROCESS_ARRIVED_DISK1";
    processName[5] = "PROCESS_EXITED_DISK1";
    processName[6] = "PROCESS_ARRIVED_DISK2";
    processName[7] = "PROCESS_EXITED_DISK2";
    processName[8] = "PROCESS_ARRIVED_NETWORK";
    processName[9] = "PROCESS_EXITED_NETWORK";
    processName[10] = "SIMULATION_FINISHED";
}
