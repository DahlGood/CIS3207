#include "Simulator.h"

void handlePROCESS_ARRIVED_SYSTEM(Event event) {
    //May be a problem with data not being updated correctly. Maybe play with pointers to avoid having to return :)
    event.eventTime = setEventTime(system_time);
    event.eventProcess = setEventProcess();
}

void handlePROCESS_EXITED_SYSTEM(Event event) {

}

void handlePROCESS_ARRIVED_CPU(Event event) {

}

void handlePROCESS_EXITED_CPU(Event event) {

}

void handlePROCESS_ARRIVED_DISK1(Event event) {

}

void handlePROCESS_EXITED_DISK1(Event event) {

}

void handlePROCESS_ARRIVED_DISK2(Event event) {

}

void handlePROCESS_EXITED_DISK2(Event event) {

}

void handlePROCESS_ARRIVED_NETWORK(Event event) {

}

void handlePROCESS_EXITED_NETWORK(Event event) {

}

void handleSIMULATION_FINISHED(Event event) {

}