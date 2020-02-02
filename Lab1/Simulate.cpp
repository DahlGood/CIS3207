#include "Simulator.h"
#include <queue>
#include <functional>

using namespace std;



int main() {
    
    //Loading Config File.
    loadConfig();

    //Random Numbers.
    Random random(getSEED());

    //Creating Queues.    
    queue<Event> CPU_queue;
    queue<Event> DISK1_queue;
    queue<Event> DISK2_queue;
    queue<Event> NETWORK_queue;
    priority_queue<Event, vector<Event>, EventComparator> event_queue; //Vector is like a dynamic array

    event_queue.push(newEvent(SIMULATION_FINISHED, getFIN_TIME(), getPID()));
    event_queue.push(newEvent(PROCESS_ARRIVED_SYSTEM, getINIT_TIME(), newProcess()));

    int event_queue_size = event_queue.size();
    for(int i = 0; i < event_queue_size; i++) {
        Event test = event_queue.top();
        cout << test.eventTime << endl;
        event_queue.pop();
    }

    int currentTime = system_time;
    system_time = getINIT_TIME();
    Event current_event;
    while(event_queue_size != 0) {
        if(CPU_queue.size() == 0) {
            current_event = newEvent(PROCESS_ARRIVED_SYSTEM);
        }

        switch(current_event.eventType) {
            case 0: handlePROCESS_ARRIVED_SYSTEM(current_event);
                break;
            case 1: handlePROCESS_EXITED_SYSTEM(current_event);
                break;
            case 2: handlePROCESS_ARRIVED_CPU(current_event);
                break;
            case 3: handlePROCESS_EXITED_CPU(current_event);
                break;
            case 4: handlePROCESS_ARRIVED_DISK1(current_event);
                break;
            case 5: handlePROCESS_EXITED_DISK1(current_event);
                break;
            case 6: handlePROCESS_ARRIVED_DISK2(current_event);
                break;
            case 7: handlePROCESS_EXITED_DISK2(current_event);
                break;
            case 8: handlePROCESS_ARRIVED_NETWORK(current_event);
                break;
            case 9: handlePROCESS_EXITED_NETWORK(current_event);
                break;
            case 10: handleSIMULATION_FINISHED(current_event);
                break;
        }
    

    }
    

    /*
    CPU_queue.push(1);
    CPU_queue.push(3);
    CPU_queue.push(2);
    CPU_queue.push(5);

    int CPU_queue_size = CPU_queue.size();
    for(int i = 0; i < CPU_queue_size; i++) {
        cout << CPU_queue.front() << endl;
        CPU_queue.pop();
    }
    */


    
    


    return 0;
}