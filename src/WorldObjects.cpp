#include <iostream>
#include <string>
#include <vector>

#include "../include/WorldObjects.h"

using namespace std;

// Constructor for WorldObjects.
// Initializes common attributes: unique ID, position (x, y), and display glyph.

WorldObjects::WorldObjects(const string& objectID, int x, int y, char g):id(objectID), pos{x, y}, glyph(g) {
    // simLog << "World Object Created (" << id << ")" << endl;
}

// Virtual destructor for WorldObjects.
// Logs the destruction of the object.

WorldObjects :: ~WorldObjects() {
    simLog << "[-OBJECT: " << id << "] destroyed." << endl;
}

// Accessor for the object's unique ID.

const string& WorldObjects::getId() const {
    return id;
}
 
// Accessor for the object's current position.

Position WorldObjects::getPosition() const {
    return pos;
}

// Accessor for the object's display glyph.

char WorldObjects::getGlyph() const {
    return glyph;
}

// Constructor for StaticObjects (non-moving items).
// Inherits from WorldObjects.

StaticObject::StaticObject(const string& objectID, int x, int y, char g) : WorldObjects(objectID, x , y, g) {
    // simLog << "StaticOject Created (" << objectID << ")" << endl;
}

// Static objects generally do not update their state, but the method is required by the interface.

void StaticObject::update() {

}

// Constructor for TrafficLight.
// Inherits from StaticObject. Starts in RED state.

TrafficLight::TrafficLight(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'R'), state(RED), timer(0) {
    simLog << "[+TRAFFIC_LIGHT: " << objectID << "] Traffic light added" << endl; 
}

// Updates the traffic light state based on a timer.
// Cycles: RED (4 ticks) -> GREEN (8 ticks) -> YELLOW (2 ticks) -> RED.
// Updates the glyph accordingly ('R', 'G', 'Y').

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

// Returns the current state (color) of the traffic light.

LightState TrafficLight::getState() const {
    return state;
}
 
// Constructor for TrafficSign.
// Inherits from StaticObject. Stores the sign text (e.g., "STOP").
 
TrafficSign::TrafficSign(const string& objectID, int x, int y, const string& txt):StaticObject(objectID, x, y, 'S'), text(txt) {
    simLog << "[+TRAFFIC_SIGN: " << objectID << "] Traffic sign added" << endl;
}

// Accessor for the sign's text content.

const string& TrafficSign::getText() const {
    return text;
}

// Constructor for StationaryVehicles (parked cars).
// Inherits from StaticObject. Displayed with 'P'.
 
StationaryVehicles::StationaryVehicles(const string& objectID, int x, int y):StaticObject(objectID, x, y, 'P') {
    simLog << "[+PARKED_CAR: " << objectID << "] Parked car added" << endl;
}
 
// Constructor for MovingObject.
// Inherits from WorldObjects. Initializes speed and direction.
 
MovingObject::MovingObject(const string& objectID, int x, int y, char g, int s, Direction d):WorldObjects(objectID, x, y , g), speed(s), direction(d) {
    // simLog << "Moving object created (" << objectID << ")" << endl;    
}
 
// Updates position by calling the move() method.

void MovingObject::update() {
    move();
}

// Updates the object's position based on its current direction and speed.

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

// Accessor for the object's speed.
 
int MovingObject::getSpeed() const {
    return speed;
}

// Accessor for the object's direction.

Direction MovingObject::getDirection() const {
    return direction;

}

// Constructor for Car (moving vehicle).
// Inherits from MovingObject. Displayed with 'C'. Speed 1.
 
Car::Car(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'C', 1, d) {
    simLog << "[+CAR: " << objectID << "] Moving car added" << endl;
}
 
// Constructor for Bike.
// Inherits from MovingObject. Displayed with 'B'. Speed 1.
 
Bike::Bike(const string& objectID, int x, int y, Direction d):MovingObject(objectID, x, y, 'B', 1, d) {
    simLog << "[+BIKE: " << objectID << "] Moving bike added" << endl;
}

    



