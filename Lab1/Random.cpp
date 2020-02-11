#include "Simulator.h"
#include <stdlib.h>
#include <math.h>

//Generates a random number between two bounds.
double getRandomBounds(int min, int max) {
    double random_num = rand() % max + min;
    return random_num;
}
