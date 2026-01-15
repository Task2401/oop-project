#include <iostream>
#include <cstdlib>

#include "../include/GridWorld.h"

using namespace std;

// Constructor for GridWorld. 
// Initializes dimensions, tick count, and logs the creation.

GridWorld::GridWorld(int dimX, int dimY):width(dimX), height(dimY), currentTick(0), car(NULL) {
    simLog << "[+WORLD: GRID] World initialized " << width << "x" << height << endl;
}

// Destructor for GridWorld.
// Cleans up all dynamically allocated world objects and the car.

GridWorld::~GridWorld() {

    for(size_t i = 0; i < objects.size(); ++i) delete objects[i];
    objects.clear();

    if (car != nullptr) {
        delete car;
        car = nullptr;
    }

    simLog << "[-WORLD] World destroyed." << endl;
}

// Finds a random position on the grid that is not occupied by any object or the car.

Position GridWorld::getRandomEmptyPosition() {
    int x, y;
    bool occupied;

    do {
        x = rand() % width;
        y = rand() % height;
        occupied = false;
 
        // Check against world objects
     
        for (size_t i = 0 ; i < objects.size(); ++i) {
            Position objectPos = objects[i]->getPosition();
            if(objectPos.x == x && objectPos.y == y) {
                occupied = true;
                break;
            }
        }

        // Check against the car
        
        if (car != nullptr) {
            Position carPos = car->getPosition();
            if (carPos.x == x && carPos.y == y) occupied = true;
        }

    } while (occupied);
    return {x, y};
}

// Populates the world with objects based on the settings provided.

void GridWorld::generateWorld(const SimSettings& settings) {
        
        // Initialize the self-driving car at a random empty position.
        
        Position carStart = getRandomEmptyPosition();
        car = new SelfDrivingCar(carStart.x, carStart.y, this, settings);   

        // Generate Traffic Lights

        for (int i = 0; i < settings.numTrafficLights; i++) {
            Position lightPos = getRandomEmptyPosition();
            string id = "LIGHT:" + to_string(i+1);
            objects.push_back(new TrafficLight(id, lightPos.x, lightPos.y));
        }

        // Generate Stop Signs
    
        for (int i = 0; i < settings.numStopSigns; i++) {
            Position signPos = getRandomEmptyPosition();
            string id = "STOP:" + to_string(i+1);
            objects.push_back(new TrafficSign(id, signPos.x, signPos.y, "STOP"));
        }

        // Generate Parked Cars
        
        for (int i = 0; i < settings.numParkedCars; i++) {
            Position parkedCarPos = getRandomEmptyPosition();
            string id = "PARKED CAR:" + to_string(i+1);
            objects.push_back(new StationaryVehicles(id, parkedCarPos.x, parkedCarPos.y));
        }

        // Generate Moving Cars with random directions
    
        for (int i = 0; i < settings.numMovingCars; i++) {
            Position movingCarPos = getRandomEmptyPosition();
            string id = "CAR:" + to_string(i+1);
            Direction dir = (Direction)(rand() % 4);
            objects.push_back(new Car(id, movingCarPos.x, movingCarPos.y, dir));
        }

        // Generate Bikes with random directions
        
        for (int i = 0; i < settings.numMovingBikes; i++) {
            Position movingBikePos = getRandomEmptyPosition();
            string id = "BIKE:" + to_string(i+1);
            Direction dir = (Direction)(rand() % 4);
            objects.push_back(new Bike(id, movingBikePos.x, movingBikePos.y, dir));
        }
}

// Updates the state of the world by one tick.
// Calls update on all objects and the car. Removes objects that go out of bounds.

void GridWorld::update() {
    currentTick++;

    for (size_t i = 0; i < objects.size(); ++i) objects[i]->update();

    auto objIndex = objects.begin();
    while (objIndex != objects.end()) {
        Position objPos = (*objIndex)->getPosition();

        if (objPos.x < 0 || objPos.x >= width || objPos.y < 0 || objPos.y >= height) {
            delete *objIndex;
            objIndex = objects.erase(objIndex);
        }
        else objIndex++;
    }
    if (car != nullptr) car->update();
}
 
// Checks if the car has moved outside the grid boundaries.
 
bool GridWorld::isCarOutOfBounds() const{
    if (car == nullptr) return true;
    Position carPos = car->getPosition();
    return (carPos.x < 0 || carPos.x >= width || carPos.y < 0 || carPos.y >= height);
}
 
// Accessor for grid width.

int GridWorld::getWidth() const {
    return width;
}

// Accessor for grid height.
 
int GridWorld::getHeight() const {
    return height;
}

// Accessor for current simulation tick.
 
int GridWorld::getTicks() const {
    return currentTick;
}

// Accessor for the list of world objects.

const vector<WorldObjects*>& GridWorld::getObjects() const {
    return objects;
}

// Accessor for the self-driving car.

SelfDrivingCar* GridWorld::getCar() {
    return car;
}