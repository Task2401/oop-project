#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

struct Position {
    int x;
    int y;
};

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
    int minConfidenceThreshold ;

    bool helpRequested;
    std::vector<Position> gpsTargets;
};



SimSettings parseArguments(int argc, char**argv);
void printHelp();


#endif