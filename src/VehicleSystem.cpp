#include "../include/VehicleSystem.h"
#include <iostream>

using namespace std;

SelfDrivingCar::SelfDrivingCar(int startX, int startY) : MovingObject("SDC", startX, startY, '@', 0,  NORTH), speedState(STOPPED) {
    cout << "SDC created (" << id << ")" << endl;
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

    else if (speedState = HALF_SPEED) {
        speedState = STOPPED;
        speed = 0;
    }
}

void SelfDrivingCar::turn(Direction newDirection) {
    direction = newDirection;
}

string SelfDrivingCar::getStatus() const {
    switch(speedState) {
        case STOPPED: return "STOPPED";
        case HALF_SPEED: return "HALF_SPEED";
        case FULL_SPEED: return "FULL_SPEED";
        default: return "UNKNOWN";
    }
}