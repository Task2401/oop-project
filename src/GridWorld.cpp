#include <iostream>
#include <cstdlib>

#include "../include/GridWorld.h"

using namespace std;

GridWorld::GridWorld(int dimX, int dimY):width(dimX), height(dimY), currentTick(0), car(NULL) {
    cout << "[+WORLD] World initialized" << width << "x" << height << endl;
}

GridWorld::~GridWorld() {

    for(int i = 0; i < objects.size(); ++i) delete objects[i];
    objects.clear();

    if (car != nullptr) {
        delete car;
        car = nullptr;
    }

    cout << "[-WORLD] World destroyed." << endl;
}

Position GridWorld::getRandomEmptyPosition() {
    int x, y;
    bool occupied;

    do {
        x = rand() % width;
        y = rand() % height;
        occupied = false;

        for (int i = 0 ; i < objects.size(); ++i) {
            Position objectPos = objects[i]->getPosition();
            if(objectPos.x == x && objectPos.y == y) {
                occupied = true;
                break;
            }
        }

        if (car != nullptr) {
            Position carPos = car->getPosition();
            if (carPos.x == x && carPos.y == y) occupied = true;
        }

    } while (occupied);
    return {x, y};
}

void GridWorld::generateWorld(const SimSettings& settings) {
        Position carStart = getRandomEmptyPosition();
        car = new SelfDrivingCar(carStart.x, carStart.y, this);   

        for (int i = 0; i < settings.numTrafficLights; i++) {
            Position lightPos = getRandomEmptyPosition();
            string id = "LIGHT:" + to_string(i+1);
            objects.push_back(new TrafficLight(id, lightPos.x, lightPos.y));
        }

        for (int i = 0; i < settings.numStopSigns; i++) {
            Position signPos = getRandomEmptyPosition();
            string id = "STOP:" + to_string(i+1);
            objects.push_back(new TrafficSign(id, signPos.x, signPos.y, "STOP"));
        }

        for (int i = 0; i < settings.numParkedCars; i++) {
            Position parkedCarPos = getRandomEmptyPosition();
            string id = "PARKED CAR:" + to_string(i+1);
            objects.push_back(new StationaryVehicles(id, parkedCarPos.x, parkedCarPos.y));
        }

        for (int i = 0; i < settings.numMovingCars; i++) {
            Position movingCarPos = getRandomEmptyPosition();
            string id = "CAR:" + to_string(i+1);
            Direction dir = (Direction)(rand() % 4);
            objects.push_back(new Car(id, movingCarPos.x, movingCarPos.y, dir));
        }

        for (int i = 0; i < settings.numMovingBikes; i++) {
            Position movingBikePos = getRandomEmptyPosition();
            string id = "BIKE:" + to_string(i+1);
            Direction dir = (Direction)(rand() % 4);
            objects.push_back(new Bike(id, movingBikePos.x, movingBikePos.y, dir));
        }
}

void GridWorld::update() {
    currentTick++;

    for (int i = 0; i < objects.size(); ++i) objects[i]->update();

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

bool GridWorld::isCarOutOfBounds() const{
    if (car == nullptr) return true;
    Position carPos = car->getPosition();
    return (carPos.x < 0 || carPos.x >= width || carPos.y < 0 || carPos.y >= height);
}

int GridWorld::getWidth() const {
    return width;
}

int GridWorld::getHeight() const {
    return height;
}

int GridWorld::getTicks() const {
    return currentTick;
}

const vector<WorldObjects*>& GridWorld::getObjects() const {
    return objects;
}

SelfDrivingCar* GridWorld::getCar() {
    return car;
}