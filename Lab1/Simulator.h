#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>

using namespace std;

extern int system_time;

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
Event newEvent(event_type type);
Event getEvent();
int setEventTime(int time);
int setEventProcess();

struct EventComparator {
    //overloading the operator function to allow us to return the result of the functions containing user defined types as booleans. Aka lets us compare data from structs.
    bool operator() (Event event1, Event event2) {
        return event1.eventTime > event2.eventTime;
    }
};

//Process
int newProcess();
int getPID();

//Random
class Random {
    private:
        int random_number;
    public:
        Random();
        Random(int SEED);
        double getRandom();
        double getRandomBounds(double min, double max);
};

/*
class EventGenerator {
    private:
        Event event;
    public:
        EventGenerator();
        Event createEvent(event_type type, int time, int PID);
        Event getEvent();
};
*/



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
void handlePROCESS_ARRIVED_SYSTEM(Event event);
void handlePROCESS_EXITED_SYSTEM(Event event);
void handlePROCESS_ARRIVED_CPU(Event event);
void handlePROCESS_EXITED_CPU(Event event);
void handlePROCESS_ARRIVED_DISK1(Event event);
void handlePROCESS_EXITED_DISK1(Event event);
void handlePROCESS_ARRIVED_DISK2(Event event);
void handlePROCESS_EXITED_DISK2(Event event);
void handlePROCESS_ARRIVED_NETWORK(Event event);
void handlePROCESS_EXITED_NETWORK(Event event);
void handleSIMULATION_FINISHED(Event event);

#endif