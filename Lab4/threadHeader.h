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

#include <fstream>

#include <chrono> 
#include <ctime> 

#include <vector>

typedef struct Counters {
    int sentSIGUSR1 = 0;
    int sentSIGUSR2 = 0;
    int receivedSIGUSR1 = 0;
    int receivedSIGUSR2 = 0;
    pthread_mutex_t mutexOne;
    pthread_mutex_t mutexTwo;
}Counters;

using namespace std;

void *generator(void* args);
void *signalUpdater(void* args);
void *reporter(void* args);
double randGenerator(double lowerBound, double upperBound);
void block_signal(int signal);
void unblock_signal(int signal);
bool maxCountReached();

