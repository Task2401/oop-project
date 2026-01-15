#include <iostream>
#include <string>
#include <vector>

#include "../include/WorldObjects.h"

using namespace std;

// --- WorldObjects ---
WorldObjects::WorldObjects(const string& objectID, int x, int y, char g):id(objectID), pos{x, y}, glyph(g) {
    // simLog << "World Object Created (" << id << ")" << endl;
}

WorldObjects :: ~WorldObjects() {
    simLog << "[-OBJECT: " << id << "] destroyed." << endl;
}

const string& WorldObjects::getId() const {
    return id;
}

Position WorldObjects::getPosition() const {
    return pos;
}

char WorldObjects::getGlyph() const {
    return glyph;
}

// --- StaticObject ---
StaticObject::StaticObject(const string& objectID, int x, int y, char g) : WorldObjects(objectID, x , y, g) {
    // simLog << "StaticOject Created (" << objectID << ")" << endl;
}

void StaticObject::update() {

}

// --- TrafficLight ---
TrafficLight::TrafficLight(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'R'), state(RED), timer(0) {
    simLog << "[+TRAFFIC_LIGHT: " << objectID << "] Traffic light added" << endl; 
}

void TrafficLight::update() {
    timer++;

    switch(state) {
        case RED:
            if (timer >= 4) {
                state = GREEN;
                glyph = 'G';
                timer = 0;
            }
            break;

        case GREEN:
            if (timer >= 8) {
                state = YELLOW;
                glyph = 'Y';
                timer = 0;
            }
            break;

        case YELLOW:
            if(timer >= 2) {
                state = RED;
                glyph = 'R';
                timer = 0;
            }
            break;
    }
}

LightState TrafficLight::getState() const {
    return state;
}

// --- TrafficSign ---
TrafficSign::TrafficSign(const string& objectID, int x, int y, const string& txt):StaticObject(objectID, x, y, 'S'), text(txt) {
    simLog << "[+TRAFFIC_SIGN: " << objectID << "] Traffic sign added" << endl;
}

const string& TrafficSign::getText() const {
    return text;
}

// --- StationaryVehicles ---
StationaryVehicles::StationaryVehicles(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'P') {
    simLog << "[+PARKED_CAR: " << objectID << "] Parked car added" << endl;
}

// --- MovingObject ---
MovingObject::MovingObject(const string& objectID, int x, int y, char g, int s, Direction d):WorldObjects(objectID, x, y , g), speed(s), direction(d) {
    // simLog << "Moving object created (" << objectID << ")" << endl;    
}

void MovingObject::update() {
    move();
}

void MovingObject::move() {
    switch(direction) {
        case NORTH:
            pos.y += speed;
            break;
        case SOUTH:
            pos.y -=speed;
            break;
        case EAST:
            pos.x += speed;
            break;
        case WEST:
            pos.x -= speed;
            break;
    }
}

int MovingObject::getSpeed() const {
    return speed;
}

Direction MovingObject::getDirection() const {
    return direction;

}

// --- Car ---
Car::Car(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'C', 1, d) {
    simLog << "[+CAR: " << objectID << "] Moving car added" << endl;
}

// --- Bike ---
Bike::Bike(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'B', 1, d) {
    simLog << "[+BIKE: " << objectID << "] Moving bike added" << endl;
}

    



