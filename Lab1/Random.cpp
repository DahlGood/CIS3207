#include "Simulator.h"
#include <stdlib.h>
#include <math.h>

void setSEED(int seed) {
    srand(seed);
}

double getRandomBounds(int min, int max) {
    double random_num = rand() % max + min;
    return random_num;
}
