#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../include/Simulation.h"

using namespace std;


SimSettings parseArguments(int argc, char**argv) {
    SimSettings settings;

    settings.seed = time(0);
    settings.dimX = 40;
    settings.dimY = 40;
    settings.numMovingCars = 3;
    settings.numMovingBikes = 4;
    settings.numParkedCars = 5;
    settings.numStopSigns = 2;
    settings.numTrafficLights = 2;
    settings.simulationTicks = 100;
    settings.minConfidenceThreshold = 40;
}