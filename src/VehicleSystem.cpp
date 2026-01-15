#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>

#include "../include/VehicleSystem.h"
#include "../include/GridWorld.h"
#include "../include/Simulation.h"

using namespace std;

SelfDrivingCar::SelfDrivingCar(int startX, int startY, const GridWorld* wolrdRef, const SimSettings& settings) : MovingObject("SDC", startX, startY, '@', 0,  NORTH), speedState(STOPPED), world(wolrdRef) {
    this->minConfidence = settings.minConfidenceThreshold;
    this->gpsTargets = settings.gpsTargets;
    this->currentTargetIndex = 0;
    
    lidar = new Lidar("LIDAR");
    radar = new Radar("RADAR");
    camera = new Camera("CAMERA");

    simLog << "[+SDC: " << id << "] SDC created sensors online" << endl;
}

SelfDrivingCar::~SelfDrivingCar() {
    delete lidar;
    delete radar;
    delete camera;

    simLog << "[-SDC: " << id << "] SDC destroyed" << endl;
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

vector<SensorReading> SelfDrivingCar::fuseSensorData(
    const vector<SensorReading>& lidarData,
    const vector<SensorReading>& radarData,
    const vector<SensorReading>& cameraData
)
{
    map<string, vector<SensorReading>> groupedData;

    for (const auto& r : lidarData)
        groupedData[r.objectID].push_back(r);

    for (const auto& r: radarData)
        groupedData[r.objectID].push_back(r);

    for (const auto& r : cameraData)
        groupedData[r.objectID].push_back(r);

    vector<SensorReading> finalResults;

    for (auto const& entry : groupedData) {
        string id = entry.first;
        vector<SensorReading> readings = entry.second;

        SensorReading merged = createEmptyReading();
        merged.objectID = id;

        double totalScore = 0.0;
        double weightedDist = 0.0;
        int count = 0;
        bool sawBike = false;

        for (const auto& r : readings) {
            totalScore += r.confidence;
            weightedDist += r.distance * r.confidence;

            if (r.type != "UNKNOWN") merged.type = r.type;

            if (r.type == "TRAFFIC_LIGHT" && r.lightState != RED)
                merged.lightState = r.lightState;

            if (r.type == "TRAFFIC_SIGN" && r.signText != "N/A")
                merged.signText = r.signText;

            if (r.speed != 0)
                merged.speed = r.speed;

            if (r.type == "BIKE")
                sawBike = true;

            count++;
        }

        if (totalScore > 0) {
            merged.distance = weightedDist / totalScore;
            merged.confidence = totalScore / count;
            merged.pos = readings[0].pos;
        }

        if (merged.confidence >= this->minConfidence || sawBike)
            finalResults.push_back(merged);
    }

    return finalResults;
}

void SelfDrivingCar::syncNavigationSystem() {
   if (world == nullptr) return;

    const vector<WorldObjects*> objects = world->getObjects();
    vector<SensorReading> lidarData = lidar->getReadings(objects, pos, direction);
    vector<SensorReading> radarData = radar->getReadings(objects, pos, direction);
    vector<SensorReading> cameraData = camera->getReadings(objects, pos, direction);

    vector<SensorReading> currentObstacles = fuseSensorData(lidarData, radarData, cameraData);

    if (currentTargetIndex >= (int) gpsTargets.size()) {
        if (speedState != STOPPED) decelerate();
        return;
    }

    Position target = gpsTargets[currentTargetIndex];

    if (pos.x == target.x && pos.y == target.y) {
        simLog << "[NAV] Reached Target #" << currentTargetIndex + 1 << " at (" << pos.x << "," << pos.y << ")" << endl;
        currentTargetIndex++;

        if (currentTargetIndex < (int) gpsTargets.size()) {
            target = gpsTargets[currentTargetIndex];
        } 
        
        else {
            if (speedState != STOPPED) decelerate();
            return;
        }
    }

    bool approachingTarget = false;
    int distToTarget = abs(pos.x - target.x) + abs(pos.y - target.y);
            
    if (distToTarget <= 5) 
        approachingTarget = true;

    int dx = target.x - pos.x;
    int dy = target.y - pos.y;

    if (abs(dx) >= abs(dy)) {
        if (dx > 0) turn(EAST);
        else if (dx < 0) turn(WEST);

        else {
             if (dy > 0) turn(NORTH);
             else if (dy < 0) turn(SOUTH);
        }
        
    } else {
        if (dy > 0) turn(NORTH);
        else if (dy < 0) turn(SOUTH);
    }

    bool safetyStop = false;
    bool cautionarySlow = false;

    for (const auto& obj : currentObstacles) {
        if (obj.type == "TRAFFIC_LIGHT") {
            if (obj.lightState == RED && obj.distance <= 3.0) {
                safetyStop = true;
                simLog << "[AUTOPILOT] Red light ahead! Stopping." << endl;
            }
            if (obj.lightState == YELLOW && obj.distance <= 3.0) cautionarySlow = true;
        }
        
        if (obj.type == "TRAFFIC_SIGN" && obj.signText == "STOP" && obj.distance <= 1.0) {
            safetyStop = true; 
            simLog << "[AUTOPILOT] STOP sign! Stopping." << endl;
        }

        if (obj.type == "CAR" || obj.type == "BIKE" || obj.type == "PARKED_CAR") {
            if (obj.distance <= 1.0) {
                safetyStop = true;
                simLog << "[AUTOPILOT] Obstacle detected (" << obj.type << ")! Stopping." << endl;
            }
        }
    }

    if (safetyStop) {
        if (speedState != STOPPED) decelerate();
    }

    else if (cautionarySlow || approachingTarget) {
        if (speedState == FULL_SPEED) decelerate();
        else if (speedState == STOPPED) accelerate();
    }

    else {
        if (speedState != FULL_SPEED) accelerate();
    }
}

void SelfDrivingCar::executeMovement() {
    move();
}

void SelfDrivingCar::update() {
   syncNavigationSystem();
   if (speed > 0) {
        move();
        simLog << "[SDC] Moved to (" << pos.x << ", " << pos.y << ")" << endl;
    }
}

bool SelfDrivingCar::hasReachedDestination() const{
    return currentTargetIndex >= (int) gpsTargets.size();
}

string SelfDrivingCar::getStatus() const {
    switch(speedState) {
        case STOPPED: return "STOPPED";
        case HALF_SPEED: return "HALF_SPEED";
        case FULL_SPEED: return "FULL_SPEED";
        default: return "UNKNOWN";
    }
}