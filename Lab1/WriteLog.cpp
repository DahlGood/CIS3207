#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include "Simulator.h"

void writeToLog(Event event) {
    //Opening a file.
    ifstream io;
    //Making sure file was opened correctly.
    io.open("log.txt");
    if(!io.is_open()) {
        cout << "Didn't open file correctly.";
        exit(EXIT_FAILURE);
    }

    //io << "At " << event.eventTime << " Process " << event.eventProcess << " " << event.eventType << endl;

    //io.close();
    
}