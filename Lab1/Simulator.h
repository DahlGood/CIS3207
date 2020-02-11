#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <queue>
#include <functional>
#include <cstdlib>

using namespace std;

//Events
enum event_type {
    PROCESS_ARRIVED_SYSTEM = 0,
    PROCESS_EXITED_SYSTEM = 1,
    PROCESS_ARRIVED_CPU = 2,
    PROCESS_EXITED_CPU = 3,
    PROCESS_ARRIVED_DISK1 = 4,
    PROCESS_EXITED_DISK1 = 5,
    PROCESS_ARRIVED_DISK2 = 6,
    PROCESS_EXITED_DISK2 = 7,
    PROCESS_ARRIVED_NETWORK = 8,
    PROCESS_EXITED_NETWORK = 9,
    SIMULATION_FINISHED = 10
};


//Event
typedef struct Event {
    event_type eventType;
    int eventTime;
    int eventProcess;
}Event;
Event newEvent(event_type type, int time, int PID);


struct EventComparator {
    //overloading the operator function to allow us to return the result of the functions containing user defined types as booleans. Aka lets us compare data from structs.
    bool operator() (Event event1, Event event2) {
        return event1.eventTime > event2.eventTime;
    }
};

//Process
int newProcess();
int getProcess();

//Random
double getRandomBounds(int min, int max);

//Writing to log
void clearLog();
void detailedLog(Event event, string action);
void setProcessNames();
void printEvent(Event event);
void printQueue(priority_queue<Event, vector<Event>, EventComparator> event_queue);

//Stats
void writeStats();
void setEQ(int size);
void setCPUQ(int size);
void setD1Q(int size);
void setD2Q(int size);
void setNQ(int size);

void setTimeCPUQ(int respTime);
void setTimeD1Q(int respTime);
void setTimeD2Q(int respTime);
void setTimeNQ(int respTime);

void setUsedCPUQ();
void setUsedD1Q();
void setUsedD2Q();
void setUsedNQ();

double getAvgSizeEQ();
double getAvgSizeCPUQ();
double getAvgSizeD1Q();
double getAvgSizeD2Q();
double getAvgSizeNQ();

double getMaxSizeEQ();
double getMaxSizeCPUQ();
double getMaxSizeD1Q();
double getMaxSizeD2Q();
double getMaxSizeNQ();

double getUtilCPU();
double getUtilD1();
double getUtilD2();
double getUtilN();

double getAvgRespCPU();
double getAvgRespD1();
double getAvgRespD2();
double getAvgRespN();

double getMaxRespCPU();
double getMaxRespD1();
double getMaxRespD2();
double getMaxRespN();
double getTHPCPU();
double getTHPD1();
double getTHPD2();
double getTHPN();

//All required functions for LoadConfig.cpp
void loadConfig();
double getSEED();
double getINIT_TIME();
double getFIN_TIME();
double getARRIVE_MIN();
double getARRIVE_MAX();
double getQUIT_PROB();
double getNETWORK_PROB();
double getCPU_MIN();
double getCPU_MAX();
double getDISK1_MIN();
double getDISK1_MAX();
double getDISK2_MIN();
double getDISK2_MAX();
double getNETWORK_MIN();
double getNETWORK_MAX();

//Event Handlers
void handlePROCESS_ARRIVED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_EXITED_SYSTEM(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_ARRIVED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_EXITED_CPU(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_ARRIVED_DISK1(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_EXITED_DISK1(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_ARRIVED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_EXITED_DISK2(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_ARRIVED_NETWORK(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handlePROCESS_EXITED_NETWORK(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);
void handleSIMULATION_FINISHED(Event &event, priority_queue<Event, vector<Event>, EventComparator> &event_queue);

#endif