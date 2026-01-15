#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Common.h"

struct SimSettings {
    int seed;
    int dimX;
    int dimY;
    int numMovingCars;
    int numMovingBikes;
    int numParkedCars;
    int numStopSigns;
    int numTrafficLights;
    int simulationTicks;
    double minConfidenceThreshold ;
    bool helpRequested;
    std::vector<Position> gpsTargets;
};

SimSettings parseArguments(int argc, char**argv);

void printHelp();

#endif