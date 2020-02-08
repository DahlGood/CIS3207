#include "Simulator.h"

Event event;
Event newEvent(event_type type, int time, int PID) {
    event.eventType = type;
    event.eventTime = time;
    event.eventProcess = PID;
    return event;
}

Event newEventB(event_type type) {
    event.eventType = type;
    event.eventTime = event.eventTime;
    event.eventProcess = event.eventProcess;
    return event;
}

Event getEvent() {
    return event;
}

int setEventTime(int time) {
    return time + getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX());
}

int setEventProcess() {
    return newProcess();
}