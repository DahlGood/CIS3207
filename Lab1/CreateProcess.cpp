#include "Simulator.h"

int PID = 1;

int newProcess() {
    PID++;
    return PID;
}

int getProcess() {
    return PID;
}
