#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include <string>
#include <vector>

#include "Common.h"
#include "WorldObjects.h"
#include "VehicleSystem.h"
#include "Simulation.h"
 
// Represents the simulation environment (grid).
// Manages all dynamic and static objects and the self-driving car.

class GridWorld {
    private:
        int width;
        int height;
        int currentTick;
        std::vector<WorldObjects*> objects;
        SelfDrivingCar* car;
        
        // Helper to find a free cell
     
        Position getRandomEmptyPosition();

    public:
        
        // Constructor
     
        GridWorld(int dimX, int dimY);
 
        // Destructor
        
        ~GridWorld();
        
        // Populates grid with objects
         
        void generateWorld(const SimSettings& settings);

        // Updates state of world and objects

        void update();
 
        // Checks boundary conditions for car
 
        bool isCarOutOfBounds() const;

        // Getters
         
        int getWidth() const;
        
        int getHeight() const;

        int getTicks() const;

        const std::vector<WorldObjects*>& getObjects() const;

        SelfDrivingCar* getCar();
};

#endif