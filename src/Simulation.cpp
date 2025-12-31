#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../include/Simulation.h"

using namespace std;

void printHelp() {
    cout << "Self-Driving Car Simulation" << endl;
    cout << "Usage: " << endl;
    cout << " --seed <n> Random seed (default : current time)" << endl;
    cout << " --dimX <n> World width (default : 40)" << endl;
    cout << " --dimY <n> World height (default : 40)" << endl;
    cout << " --numMovingCars <n> Number of moving cars (default : 3)" << endl;
    cout << " --numMovingBikes <n> Number of moving bikes (default : 4)" << endl;
    cout << " --numParkedCars <n> Number of parked cars (default : 5)" << endl;
    cout << " --numStopSigns <n> Number of stop signs (default : 2)" << endl;
    cout << " --numTrafficLights <n> Number of traffic lights (default : 2)" << endl;
    cout << " --simulationTicks <n> Maximum simulation ticks (default : 100)" << endl;
    cout << " --minConfidenceThreshold <n> Minimum confidence cutoff (default : 40)" << endl;
    cout << " --gps <x1> <y1> [x2 y2 ...] GPS target coordinates (required)" << endl;
    cout << " --help Show this help message" << endl << endl;
    cout << "Example usage:" << endl;
    cout << " ./ oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
}

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

    settings.helpRequested = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--help") {
            settings.helpRequested = true;
            printHelp();
            return;
        }

        else if (arg == "--seed") {
            if ((i + 1) < argc) settings.seed = atoi(argv[i++]);
        }

        else if (arg == "--dimX") {
            if ((i + 1) < argc) settings.dimX = atoi(argv[i++]);
        }

        else if (arg == "--dimY") {
            if ((i + 1) < argc) settings.dimY = atoi(argv[i++]);
        }

        else if (arg == "--numMovingCars") {
            if ((i + 1) < argc) settings.numMovingCars = atoi(argv[i++]);
        }

        else if (arg == "--numMovingBikes") {
            if ((i + 1) < argc) settings.numMovingBikes = atoi(argv[i++]);
        }

        else if (arg == "--numParkedCars") {
            if ((i + 1) < argc) settings.numParkedCars = atoi(argv[i++]);
        }

        else if (arg == "--numStopSigns") {
            if ((i + 1) < argc) settings.numStopSigns = atoi(argv[i++]);
        }

        else if (arg == "--numTrafficLights") {
            if ((i + 1) < argc) settings.numTrafficLights = atoi(argv[i++]);
        }

        else if (arg == "--simulationTicks") {
            if ((i + 1) < argc) settings.simulationTicks = atoi(argv[i++]);
        }

        else if (arg == "--minConfidenceThreshold") {
            if ((i + 1) < argc) settings.minConfidenceThreshold = atoi(argv[i++]);
        }

        else if (arg == "--gps") {
            while ((i + 2) < argc) {
                string nextCheck = argv[i + 1];
                if (nextCheck.substr(0, 2) == "--") {
                    break;
                }

                Position pos;
                pos.x = atoi(argv[i++]);
                pos.y = atoi(argv[i++]);
                settings.gpsTargets.push_back(pos);
            }
        }
    }
    return settings;
}