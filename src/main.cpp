#include <iostream>
#include <vector>
#include <string>

#include "../include/Simulation.h"
#include "../include/GridWorld.h"
#include "../include/VehicleSystem.h"
#include "../include/WorldObjects.h"
#include "../include/Common.h"

using namespace std;

ofstream simLog;

char getCellGlyph(GridWorld& world, int x, int y) {
    SelfDrivingCar* car = world.getCar();
    if (car != nullptr) {
        Position carPos = car->getPosition();
        if (carPos.x == x && carPos.y == y) return '@';
    }

    const vector<WorldObjects*>& worldObjects = world.getObjects();
    vector<char> glyphsInCell;

    for (const auto& obj : worldObjects) {
        Position objPos = obj->getPosition();
        if (objPos.x == x && objPos.y == y) 
            glyphsInCell.push_back(obj->getGlyph());
    }

    if(glyphsInCell.empty()) return '.';

    for (int i = 0; i < glyphsInCell.size(); i++) 
        if (glyphsInCell[i] == 'R') return 'R';

    for (int i = 0; i < glyphsInCell.size(); i++)
        if (glyphsInCell[i] == 'Y') return 'Y';

    for (int i = 0; i < glyphsInCell.size(); i++) 
        if (glyphsInCell[i] == 'S') return 'S';

    for (int i = 0; i < glyphsInCell.size(); i++)
        if (glyphsInCell[i] == 'B') return 'B';
    
    for (int i = 0; i < glyphsInCell.size(); i++)
        if (glyphsInCell[i] == 'C') return 'C';
        
    for (int i = 0; i < glyphsInCell.size(); i++)
        if (glyphsInCell[i] == 'G') return 'G';

    for (int i = 0; i < glyphsInCell.size(); i++)
        if (glyphsInCell[i] == 'P') return 'P';

    return '?';
}

void visualizationFull(GridWorld& world) {
    int width = world.getWidth();
    int height = world.getHeight();

    cout << "--- FULL MAP (Tick: " << world.getTicks() << ") ---" << endl;

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            cout << getCellGlyph(world, x, y) << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------" << endl;
}

void visualizationPov(GridWorld& world, int radius) {
    SelfDrivingCar* car = world.getCar();
    if (car == nullptr) return;

    Position carPos = car->getPosition();
    int width = world.getWidth();
    int height = world.getHeight();

    cout << "--- POV MAP (Radius: " << radius << ") ---" << endl;

    int startY = carPos.y + radius;
    int endY = carPos.y - radius;
    int startX = carPos.x - radius;
    int endX = carPos.x + radius;

    for (int y = startY; y >= endY; --y) {
        for (int x = startX; x <= endX; ++x) {
            if (x < 0 || x >= width || y < 0 || y >= height) cout << "X ";
            else cout << getCellGlyph(world, x, y) << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------" << endl;
}

int main(int argc, char**argv) {
    simLog.open("logs/oopproj_2025.log");

    if (!simLog.is_open()) {
        cout << "Error: Could not open log file in logs/ directory!" << endl;
        cout << "Make sure you are running the program from the project root and the 'logs' folder exists." << endl;
        return 1;
    }

    SimSettings settings = parseArguments(argc, argv);

    if (settings.helpRequested) {
        simLog.close();
        return 0;
    }

    if (settings.gpsTargets.empty()) {
        cout << "Error: No GPS targets provided. Use --gps <x> <y> ..." << endl;
        simLog << "Error: No GPS targets provided." << endl;
        return 1;
    }

    srand(settings.seed);

    GridWorld world(settings.dimX, settings.dimY);
    world.generateWorld(settings);

    visualizationFull(world);

    bool simulationRunning = true;

    while (simulationRunning && world.getTicks() < settings.simulationTicks) {
        world.update();
        visualizationPov(world, 5);
        SelfDrivingCar* car = world.getCar();

        if (world.isCarOutOfBounds()) {
            cout << "Simulation Ended: Car went out of bounds!" << endl;
            simLog << "Simulation Ended: Car went out of bounds!" << endl;
            simulationRunning = false;
        }

        else if (car != nullptr && car->hasReachedDestination() && car->getSpeed() == 0) {
            cout << "Simulation Ended: Destination Reached!" << endl;
            simLog << "Simulation Ended: Destination Reached!" << endl;
            simulationRunning = false;
        }

        else if (car == nullptr) {
            cout << "Simulation Ended: Car is gone!" << endl;
            simulationRunning = false;
        }

    }

    visualizationFull(world);

    cout << "Simulation finished after " << world.getTicks() << " ticks." << endl;
    simLog << "Simulation finished after " << world.getTicks() << " ticks." << endl;

    simLog.close();
    return 0;
}