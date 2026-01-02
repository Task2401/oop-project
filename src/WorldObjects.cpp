#include <iostream>
#include <string>
#include <vector>

#include "../include/WorldObjects.h"

using namespace std;

WorldObjects::WorldObjects(const string& objectID, int x, int y, char g):id(objectID), pos{x, y}, glyph(g) {
    cout << "World Object Created (" << id << ")" << endl;
}

WorldObjects :: ~WorldObjects() {
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

StaticObject::StaticObject(const string& objectID, int x, int y, char g) : WorldObjects(objectID, x , y, g) {
    cout << "StaticOject Created (" << objectID << ")" << endl;
}

void StaticObject::update() {

}

MovingObject::MovingObject(const string& objectID, int x, int y, char g, int s, Direction d):WorldObjects(objectID, x, y , g), speed(s), direction(d) {
    cout << "Moving object created (" << objectID << ")" << endl;    
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

TrafficLight::TrafficLight(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'R'), state(RED), timer(0) {
    cout << "Traffic light added (" << objectID << ")" << endl; 
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

TrafficSign::TrafficSign(const string& objectID, int x, int y, const string& txt):StaticObject(objectID, x, y, 'S'), text(txt) {
    cout << "Traffic sign added (" << objectID << ")" << endl;

}

const string& TrafficSign::getText() const {
    return text;
}

StationaryVehicles::StationaryVehicles(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'P') {
    cout << "Parked car added (" << objectID << ")" << endl;
}

Car::Car(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'C', 2, d) {
    cout << "Moving car added (" << objectID << ")" << endl;
}

Bike::Bike(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'B', 1, d) {
    cout << "Moving bike added (" << objectID << ")" << endl;
}

    



