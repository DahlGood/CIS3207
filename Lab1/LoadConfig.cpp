#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include "Simulator.h"

using namespace std;

double SEED;
double INIT_TIME;
double FIN_TIME;
double ARRIVE_MIN;
double ARRIVE_MAX;
double QUIT_PROB;
double NETWORK_PROB;
double CPU_MIN;
double CPU_MAX;
double DISK1_MIN;
double DISK1_MAX;
double DISK2_MIN;
double DISK2_MAX;
double NETWORK_MIN;
double NETWORK_MAX;

double config[15];

int hello() {
    return 20;
}

void loadConfig() {
    
    char file_name[] = "config.txt";

    //Opening a file.
    ifstream io;
    //Making sure file was opened correctly.
    io.open(file_name);
    if(!io.is_open()) {
        cout << "Didn't open file correctly.";
        exit(EXIT_FAILURE);
    }

    char buffer[50];
    io >> buffer;
    int i = 0;
    //good() is a built in function that tells us if we have reached the end of the file.
    while(io.good()) {

        //Splits line by "=" and stores whats after the "=" in the config array.
        char* tokenized = strtok(buffer, "=");
        tokenized = strtok(NULL, "=");
        config[i] = atof(tokenized);

        //Puts next line into buffer.
        io >> buffer;

        i++;
    }

}

//Getters
double getSEED() {
    return config[0];
}

double getINIT_TIME() {
    return config[1];
}

double getFIN_TIME() {
    return config[2];
}

double getARRIVE_MIN() {
    return config[3];
}

double getARRIVE_MAX() {
    return config[4];
}

double getQUIT_PROB() {
    return config[5];
}

double getNETWORK_PROB() {
    return config[6];
}

double getCPU_MIN() {
    return config[7];
}

double getCPU_MAX() {
    return config[8];
}

double getDISK1_MIN() {
    return config[9];
}

double getDISK1_MAX() {
    return config[10];
}

double getDISK2_MIN() {
    return config[11];
}

double getDISK2_MAX() {
    return config[12];
}

double getNETWORK_MIN() {
    return config[13];
}

double getNETWORK_MAX() {
    return config[14];
}
