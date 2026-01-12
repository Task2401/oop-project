#include <iostream>
#include <cstdlib>
#include <cmath>

#include "../include/Sensors.h"

using namespace std;

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

Sensor::Sensor(const string& sensorID, double accuracy):id(sensorID), baseAccuracy(accuracy) {};
Sensor::~Sensor() {};

string Sensor::getId() const {
    return id;
}

double Sensor::calculateDistance(Position p1, Position p2) const {
    return (double)(abs(p1.x - p2.x) + abs(p1.y - p2.y));
}

double Sensor::applyNoise(double conf) const {
    double noise = ((rand() % 100) / 1000.0) - 0.05;
    double finalConf = conf + noise;

    if (finalConf > 1.0) return 1.0;
    if (finalConf < 0.0) return 0.0;

    return finalConf;
}

Lidar::Lidar(const string& sensorId) : Sensor(sensorId, 0.99) {
    cout << "[+LIDAR: " << id << "] Lidar sensor ready Sensing with pew pews!" << endl;
}

vector<SensorReading> Lidar::getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;

    for (int i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        Position objPos = obj->getPosition();
        double dist = calculateDistance(carPos, objPos);

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

            double distFactor = 1.0 - (dist / 9.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}

Radar::Radar(const string& sensorId) : Sensor(sensorId, 0.99) {
    cout << "[+RADAR: " << id << "] Radar sensor ready I'm a Radio star!" << endl;
}

vector<SensorReading> Radar::getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;

    for (int i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        char objSymbol = obj->getGlyph();

        if (objSymbol != 'C' && objSymbol != 'B') continue;

        MovingObject* movObj = (MovingObject*)obj;
        Position movObjPos = movObj->getPosition();
        bool inRange = false;

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

            double distFactor = 1.0 - (r.distance / 12.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}

Camera::Camera(const string& sensorId) : Sensor(sensorId, 0.95) {
    cout << "[+CAMERA: " << id << "] Camera sensor ready Say cheese!" << endl;
}

vector<SensorReading> Camera:: getReadings(const vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) {
    vector<SensorReading> readings;
    int minX = 0, maxX = 0, minY = 0, maxY = 0;

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

    for (int i = 0; i < allObjects.size(); i++) {
        WorldObjects* obj = allObjects[i];
        Position objPos = obj->getPosition();

        if (objPos.x >= minX && objPos.x <= maxX && objPos.y >= minY && objPos.y <= maxY) {

            SensorReading r = createEmptyReading();
            r.objectID = obj->getId();
            r.pos = objPos;
            r.distance = calculateDistance(carPos, objPos);

            char objSymbol = obj->getGlyph();

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

            double distFactor = 1.0 - (r.distance / 8.0);
            if (distFactor < 0.0) distFactor = 0.0;

            r.confidence = applyNoise(baseAccuracy * distFactor);
            readings.push_back(r);
        }
    }
    return readings;
}