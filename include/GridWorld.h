#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include <string>
#include <vector>

#include "Common.h"
#include "WorldObjects.h"
#include "VehicleSystem.h"
#include "Simulation.h"

class GridWorld {
    private:
        int width;
        int height;
        int currentTick;
        std::vector<WorldObjects*> objects;
        SelfDrivingCar* car;
        Position getRandomEmptyPosition();

    public:
        GridWorld(int dimX, int dimY);

        ~GridWorld();

        void generateWorld(const SimSettings& settings);

        void update();

        bool isCarOutOfBounds() const;

        int getWidth() const;
        
        int getHeight() const;

        int getTicks() const;

        const std::vector<WorldObjects*>& getObjects() const;

        SelfDrivingCar* getCar();
};

#endif