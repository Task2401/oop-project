#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Common.h"
 
// Stores all configuration parameters for the simulation

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
 
// Parses command line arguments into SimSettings

SimSettings parseArguments(int argc, char**argv);

// Displays help message to console
 
void printHelp();

#endif