#include <iostream>
#include <cstdlib>
#include <cmath>

#include "../include/Sensors.h"

using namespace std;

// 
// Utility function to initialize a SensorReading struct with default/placeholder values.
// Ensures that no fields are left uninitialized.
// 
SensorReading createEmptyReading() {
    SensorReading readings;

    readings.objectID = "";
    readings.type = "UNKNOWN";
    readings.pos = {-1, -1};
    readings.distance = -1.0;
    readings.confidence = 0.0;
    readings.speed = 0;
    readings.direction = NORTH;
    readings.signText = "N/A";
    readings.lightState = RED;

    return readings;
}

// Constructor for the abstract base Sensor class.
// Initializes the sensor ID and its base accuracy.

Sensor::Sensor(const string& sensorID, double accuracy):id(sensorID), baseAccuracy(accuracy) {};

// Virtual destructor for Sensor.

Sensor::~Sensor() {};
 
// Accessor for the sensor's ID.

string Sensor::getId() const {
    return id;
}

// Helper function to calculate the Manhattan distance between two points.
// Used by all sensors to determine range and proximity.

double Sensor::calculateDistance(Position p1, Position p2) const {
    return (double)(abs(p1.x - p2.x) + abs(p1.y - p2.y));
}

// Simulates sensor noise by applying a random value to the confidence level.
// Ensures that the confidence stays within the [0.0, 1.0] range.

double Sensor::applyNoise(double conf) const {
    double noise = ((rand() % 100) / 1000.0) - 0.05;
    double finalConf = conf + noise;

    if (finalConf > 1.0) return 1.0;
    if (finalConf < 0.0) return 0.0;

    return finalConf;
}

// Constructor for Lidar.
// Lidar has high accuracy (0.99) and logs its activation.

Lidar::Lidar(const string& sensorId) : Sensor(sensorId, 0.99) {
    simLog << "[+LIDAR: " << id << "] Lidar sensor ready Sensing with pew pews!" << endl;
}

// Destructor for Lidar. Logs deactivation.

Lidar::~Lidar() {
    simLog << "[-LIDAR: " << id << "] Lidar offline" << endl;
}

// Scans the environment for objects within a 4x4 box around the car.
// Detects all types of objects and provides type-specific details.

vector<SensorReading> Lidar::getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;

    for (size_t i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        Position objPos = obj->getPosition();
        double dist = calculateDistance(carPos, objPos);
        
        // Check if object is within Lidar range (Manhattan distance <= 4 on both axes)
        
        if (abs(objPos.x - carPos.x) <= 4 && abs(objPos.y - carPos.y) <= 4) {
            
            if (objPos.x == carPos.x && objPos.y == carPos.y) continue;
            
            SensorReading r = createEmptyReading();
            r.objectID = obj->getId();
            r.pos = objPos;
            r.distance = dist;

            char objSymbol = obj->getGlyph();

            switch (objSymbol) {
                case 'C':
                    r.type = "CAR";
                    break;
                
                case 'B':
                    r.type = "BIKE";
                    break;

                case 'S':
                    r.type = "TRAFFIC_SIGN";
                    break;
                
                case 'P':
                    r.type = "PARKED_CAR";
                    break;

                case 'R':
                    r.type = "TRAFFIC_LIGHT";
                    break;

                case 'G':
                    r.type = "TRAFFIC_LIGHT";
                    break;

                case 'Y':
                    r.type = "TRAFFIC_LIGHT";
                    break;

                default:
                    r.type = "UNKNOWN";
                    break;
            }

            // Calculate confidence based on distance and base accuracy.
            
            double distFactor = 1.0 - (dist / 9.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}

// Constructor for Radar.
// Radar has high accuracy (0.99) and logs its activation.

Radar::Radar(const string& sensorId) : Sensor(sensorId, 0.99) {
    simLog << "[+RADAR: " << id << "] Radar sensor ready I'm a Radio star!" << endl;
}

// Destructor for Radar. Logs deactivation.

Radar::~Radar() {
    simLog << "[-RADAR: " << id << "] Radar offline" << endl;
}

// Scans for MOVING objects (Cars, Bikes) in a long range ahead of the car.
// The range depends on the car's orientation (up to 12 units ahead).

vector<SensorReading> Radar::getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;

    for (size_t i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        char objSymbol = obj->getGlyph();

        // Radar only detects moving vehicles
        
        if (objSymbol != 'C' && objSymbol != 'B') continue;

        MovingObject* movObj = (MovingObject*)obj;
        Position movObjPos = movObj->getPosition();
        bool inRange = false;

        // Detection logic based on car direction: straightforward beam
    
        if (carDir == NORTH) {
            if (movObjPos.x == carPos.x && movObjPos.y > carPos.y && movObjPos.y <= carPos.y + 12) inRange = true;
        }

        else if (carDir == SOUTH) {
            if (movObjPos.x == carPos.x && movObjPos.y < carPos.y && movObjPos.y >= carPos.y - 12) inRange = true;
        }

        else if (carDir == EAST) {
            if (movObjPos.y == carPos.y && movObjPos.x > carPos.x && movObjPos.x <= carPos.x + 12) inRange = true;
        }

        else if (carDir == WEST) {
            if (movObjPos.y == carPos.y && movObjPos.x < carPos.x && movObjPos.x >= carPos.x - 12) inRange = true;
        }

        if (inRange) {
            SensorReading r = createEmptyReading();
            r.objectID = movObj->getId();

            if (objSymbol == 'B') r.type = "BIKE";
            else r.type = "CAR";

            r.pos = movObjPos;
            r.distance = calculateDistance(carPos, movObjPos);
            r.speed = movObj->getSpeed();
            r.direction = movObj->getDirection();

            // Calculate confidence based on distance and base accuracy.
            
            double distFactor = 1.0 - (r.distance / 12.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}

// Constructor for Camera.
// Camera has lower accuracy (0.95) than Lidar/Radar and logs its activation.

Camera::Camera(const string& sensorId) : Sensor(sensorId, 0.95) {
    simLog << "[+CAMERA: " << id << "] Camera sensor ready Say cheese!" << endl;
}

// Destructor for Camera. Logs deactivation.

Camera::~Camera() {
    simLog << "[-CAMERA: " << id << "] Camera offline" << endl;
}

// Scans a rectangular area in front of the car.
// Capable of identifying Traffic Lights states and Sign text.

vector<SensorReading> Camera:: getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    
    // Define Field of View (FOV) based on car direction
    
    switch (carDir) {
        case NORTH:
            minX = carPos.x - 3;
            maxX = carPos.x + 3;
            minY = carPos.y + 1;
            maxY = carPos.y + 7;
            break;

        case SOUTH:
            minX = carPos.x - 3;
            maxX = carPos.x + 3;
            minY = carPos.y - 7;
            maxY = carPos.y - 1;
            break;
        
        case EAST:
            minX = carPos.x + 1;
            maxX = carPos.x + 7;
            minY = carPos.y - 3;
            maxY = carPos.y + 3;
            break;

        case WEST:
            minX = carPos.x - 7;
            maxX = carPos.x - 1;
            minY = carPos.y - 3;
            maxY = carPos.y + 3;
            break;
    }

    for (size_t i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        Position objPos = obj->getPosition();
        
        // Check if object is within FOV
        
        if (objPos.x >= minX && objPos.x <= maxX && objPos.y >= minY && objPos.y <= maxY) {

            SensorReading r = createEmptyReading();
            r.objectID = obj->getId();
            r.pos = objPos;
            r.distance = calculateDistance(carPos, objPos);

            char objSymbol = obj->getGlyph();

            // Identify object and extract specific details (light state, sign text, etc.)
        
            if (objSymbol == 'R' || objSymbol == 'G' ||objSymbol == 'Y') {
                r.type = "TRAFFIC_LIGHT";
                TrafficLight* tl = (TrafficLight*)obj;
                r.lightState = tl->getState();
            }

            else if (objSymbol == 'S') {
                r.type = "TRAFFIC_SIGN";
                TrafficSign* ts = (TrafficSign*)obj;
                r.signText = ts->getText();
            }

            else if (objSymbol == 'C' || objSymbol == 'B') {
                if (objSymbol == 'B') r.type = "BIKE";
                else r.type = "CAR";

                MovingObject* movObj = (MovingObject*)obj;
                r.speed = movObj->getSpeed();
                r.direction = movObj->getDirection();
            }

            else if (objSymbol == 'P') r.type = "PARKED_CAR";
            
            else r.type = "UNKNOWN";
            
            // Calculate confidence based on distance and base accuracy.
            
            double distFactor = 1.0 - (r.distance / 8.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}