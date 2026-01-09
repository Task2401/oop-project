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
