#include "Simulator.h"

int PID = 1;

int newProcess() {
    PID++;
    return PID;
}

int getPID() {
    return PID;
}