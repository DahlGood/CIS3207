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

void printConfig(double* config) {
    //Opening a file.
    ofstream io;
    //Making sure file was opened correctly.
    io.open("detailed_log.txt", ios::app);
    if(!io.is_open()) {
        cout << "Didn't open file correctly.";
        exit(EXIT_FAILURE);
    }

    io << "SEED= " << config[0] << endl;
    io << "INIT_TIME= " << config[1] << endl;
    io << "FIN_TIME= " << config[2] << endl;
    io << "ARRIVE_MIN= " << config[3] << endl;
    io << "ARRIVE_MAX= " << config[4] << endl;
    io << "QUIT_PROB= " << config[5] << endl;
    io << "NETWORK_PROB= " << config[6] << endl;
    io << "CPU_MIN= " << config[7] << endl;
    io << "CPU_MAX= " << config[8] << endl;
    io << "DISK1_MIN= " << config[9] << endl;
    io << "DISK1_MAX= " << config[10] << endl;
    io << "DISK2_MIN= " << config[11] << endl;
    io << "DISK2_MIN= " << config[12] << endl;
    io << "NETOWORK_MIN= " << config[13] << endl;
    io << "NETOWORK_MAX= " << config[14] << endl;
    io << "\n" << endl;

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
    remove("stats.txt");

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
