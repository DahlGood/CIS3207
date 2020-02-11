#include "Simulator.h"
#include <functional>

using namespace std;

int main() {

    //Loading Config File.
    clearLog();
    loadConfig();
    setProcessNames();
    printConfig(getConfig());

    //Random Numbers.
    srand(getSEED());

    //Creating Queues.    
    priority_queue<Event, vector<Event>, EventComparator> event_queue; //Vector is like a dynamic array

    event_queue.push(newEvent(SIMULATION_FINISHED, getFIN_TIME(), 1));
    event_queue.push(newEvent(PROCESS_ARRIVED_SYSTEM, getINIT_TIME(), newProcess()));

    int event_queue_size = event_queue.size();
    Event current_event;
    while(event_queue_size > 0) {

        //This is where we van guarentee the size of the event queue after each handler is run, so we'll put the event queue information gatherer here.
        setEQ(event_queue.size());

        //Saves the event on the queue that is to be processed.
        current_event = event_queue.top();
        
        //Removes the event from the queue.
        event_queue.pop();
        
        event_queue_size = event_queue.size();

        //Processes the event.
        switch(current_event.eventType) {
            case 0: handlePROCESS_ARRIVED_SYSTEM(current_event, event_queue);
                break;
            case 1: handlePROCESS_EXITED_SYSTEM(current_event, event_queue);
                break;
            case 2: handlePROCESS_ARRIVED_CPU(current_event, event_queue);
                break;
            case 3: handlePROCESS_EXITED_CPU(current_event, event_queue);
                break;
            case 4: handlePROCESS_ARRIVED_DISK1(current_event, event_queue);
                break;
            case 5: handlePROCESS_EXITED_DISK1(current_event, event_queue);
                break;
            case 6: handlePROCESS_ARRIVED_DISK2(current_event, event_queue);
                break;
            case 7: handlePROCESS_EXITED_DISK2(current_event, event_queue);
                break;
            case 8: handlePROCESS_ARRIVED_NETWORK(current_event, event_queue);
                break;
            case 9: handlePROCESS_EXITED_NETWORK(current_event, event_queue);
                break;
            case 10: handleSIMULATION_FINISHED(current_event, event_queue);
                break;
        }
    

    }


    return 0;
}