#include "Simulator.h"
#include <stdlib.h>
#include <math.h>

Random::Random() {
    srand(time(NULL));
    random_number = rand();
}

Random::Random(int SEED) {
    srand(SEED);
    random_number = rand();
}

double Random::getRandom() {
    return random_number;
}

double Random::getRandomBounds(double min, double max) {
    return fmod(random_number, max-min) + min;
}