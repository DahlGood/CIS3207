#include "Simulator.h"

//Creates an event.
Event newEvent(event_type type, int time, int PID) {
    Event event;
    event.eventType = type;
    event.eventTime = time;
    event.eventProcess = PID;
    return event;
}
