#include <iostream>

#include "../include/VehicleSystem.h"
#include "../include/GridWorld.h"

using namespace std;

SelfDrivingCar::SelfDrivingCar(int startX, int startY, const GridWorld* wolrdRef) : MovingObject("SDC", startX, startY, '@', 0,  NORTH), speedState(STOPPED), world(wolrdRef) {
    lidar = new Lidar("LIDAR");
    radar = new Radar("RADAR");
    camera = new Camera("CAMERA");
    cout << "SDC created (" << id << ") sensors online" << endl;
}

SelfDrivingCar::~SelfDrivingCar() {
    delete lidar;
    delete radar;
    delete camera;
}

void SelfDrivingCar::accelerate() {
    if (speedState == STOPPED) {
        speedState = HALF_SPEED;
        speed = 1;
    }

    else if (speedState == HALF_SPEED) {
        speedState = FULL_SPEED;
        speed = 2;
    }
}

void SelfDrivingCar::decelerate() {
    if (speedState == FULL_SPEED) {
        speedState = HALF_SPEED;
        speed = 1;
    }

    else if (speedState == HALF_SPEED) {
        speedState = STOPPED;
        speed = 0;
    }
}

void SelfDrivingCar::turn(Direction newDirection) {
    direction = newDirection;
}

void SelfDrivingCar::collectSensorData() {
    if (world != nullptr) {
        const vector<WorldObjects*>& objects = world->getObjects();
        lidar->getReadings(objects, pos, direction);
        radar->getReadings(objects, pos, direction);
        camera->getReadings(objects, pos, direction);
    }
}

void SelfDrivingCar::syncNavigationSystem() {

}

void SelfDrivingCar::executeMovement() {
    move();
    if (speed == 0) accelerate();
}

string SelfDrivingCar::getStatus() const {
    switch(speedState) {
        case STOPPED: return "STOPPED";
        case HALF_SPEED: return "HALF_SPEED";
        case FULL_SPEED: return "FULL_SPEED";
        default: return "UNKNOWN";
    }
}