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