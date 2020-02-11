#include "Simulator.h"

int PID = 1;

//Increments the PID
int newProcess() {
    PID++;
    return PID;
}

//Returns the current PID
int getProcess() {
    return PID;
}
