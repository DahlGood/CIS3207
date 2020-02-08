#include "Simulator.h"
#include <stdlib.h>
#include <math.h>

int random_number;
void setSEED(int seed) {
    srand(seed);
    random_number = rand();
}

double getRandom() {
    return random_number;
}

double getRandomBounds(double min, double max) {
    return fmod(random_number, max-min) + min;
}
