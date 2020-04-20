#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <errno.h>

//Shared Memory
#include <sys/ipc.h>
#include <sys/shm.h>


typedef struct Counters {
    int sentSIGUSR1 = 0;
    int sentSIGUSR2 = 0;
    int receivedSIGUSR1 = 0;
    int receivedSIGUSR2 = 0;
    pthread_mutex_t mutexOne;
    pthread_mutex_t mutexTwo;
}Counters;

using namespace std;

void signalGenerator(int signal);
void signalHandler(int signal);
//void signalTwoHandler(Counters *count, int signal);
void reporter(int signal);
void block_signal(int x);
double randGenerator(double lowerBound, double upperBound);
bool maxCountReached();