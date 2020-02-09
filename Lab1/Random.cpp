#include "Simulator.h"
#include <stdlib.h>
#include <math.h>

void setSEED(int seed) {
    srand(seed);
}

double getRandom() {
    return rand();
}

double getRandomBounds(int min, int max) {
    //double random_num = (fmod(rand(), max-min) + min)/100;
    double random_num = rand() % max + min;
    cout << "randomsss " << random_num << endl;
    return random_num;
}
