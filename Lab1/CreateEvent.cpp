#include "Simulator.h"

Event newEvent(event_type type, int time, int PID) {
    Event event;
    event.eventType = type;
    event.eventTime = time;
    event.eventProcess = PID;
    return event;
}
