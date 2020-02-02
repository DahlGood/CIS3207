#include "Simulator.h"

Event event;
Event newEvent(event_type type, int time, int PID) {
    event.eventType = type;
    event.eventTime = time;
    event.eventProcess = PID;
    return event;
}

Event newEvent(event_type type) {
    event.eventType = type;
    return event;
}

Event getEvent() {
    return event;
}

int setEventTime(int time) {
    Random random(getSEED());
    return time + random.getRandomBounds(getARRIVE_MIN(), getARRIVE_MAX());
}

int setEventProcess() {
    return newProcess();
}