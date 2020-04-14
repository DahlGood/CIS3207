#include "processHeader.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TIME_TO_RUN 30
#define SUB_PROCESSES 8

int main() {


    //Initialization of globally shared variables
    int sentSIGUSR1 = 0;
    int sentSIGUSR2 = 0;
    int receivedSIGUSR1 = 0;
    int receivedSIGUSR2 = 0;

    //Set-up mutexes

    //Controlling execution duration of the program


    int processType;
    for(int i = 0; i < SUB_PROCESSES; i++) {
        processType = fork();
        if(processType < 4) {
            //Creates 4 signal handlers
        }
        else if(processType >= 4 && processType < 8) {
            //Creates 3 signal generators
        }
        else if(processType == 8) {
            //Creates 1 reporting process
        }
        else {
            //This is the 1 parent process.

            /*
                If current time < 30 continue running, otherwise close everything.
            */
            wait(NULL);
        }
    }


}

void signalGenerator() {
    
    srand(time(NULL));

    while(true) {

        //Do stuff
        if(randGenerator(0, 1) < 0.5) {
            //Signal SIGUSR1
            //sentSIGUSR1++ INSURE MUTEX
        }
        else {
            //Signal SIGUSR2
            //sentSIGUSR2++ INSURE MUTEX
        }

        sleep(randGenerator(0.01, 0.1));

    }
}

void signalOneHandler(int signal) {
    //Ensure MUTEXs
    //This is not how it will actually be done. Find a way to have a process only accept certain signals.
    if(signal = SIGUSR1) {

    }
    else if(signal = SIGUSR2) {

    }
}

void reporter() {
    while(true) {

    }
}

void handleSIG(int signal) {
    
}

double randGenerator(double lowerBound, double upperBound) {
    return (fmod(rand(), upperBound) + lowerBound);
}
