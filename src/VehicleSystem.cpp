#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>

#include "../include/VehicleSystem.h"
#include "../include/GridWorld.h"
#include "../include/Simulation.h"

using namespace std;

// SDC Constructor.
// Initializes the car at a starting position, loads GPS settings, and initializes sensors.

SelfDrivingCar::SelfDrivingCar(int startX, int startY, const GridWorld* wolrdRef, const SimSettings& settings) : MovingObject("SDC", startX, startY, '@', 0,  NORTH), speedState(STOPPED), world(wolrdRef) {
    this->minConfidence = settings.minConfidenceThreshold;
    this->gpsTargets = settings.gpsTargets;
    this->currentTargetIndex = 0;
    
    lidar = new Lidar("LIDAR");
    radar = new Radar("RADAR");
    camera = new Camera("CAMERA");

    simLog << "[+SDC: " << id << "] SDC created sensors online" << endl;
}

// SDC Destructor.
// Cleans up dynamically allocated sensors.

SelfDrivingCar::~SelfDrivingCar() {
    delete lidar;
    delete radar;
    delete camera;

    simLog << "[-SDC: " << id << "] SDC destroyed" << endl;
}

// Increases speed state: STOPPED -> HALF_SPEED -> FULL_SPEED.

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

// Decreases speed state: FULL_SPEED -> HALF_SPEED -> STOPPED.

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

// Sets a new direction for the car to move in.

void SelfDrivingCar::turn(Direction newDirection) {
    direction = newDirection;
}

// Sensor Fusion Algorithm.
// Takes data from Lidar, Radar, and Camera, groups readings by Object ID, and merges them.
// Calculates a weighted average for distance and confidence.
// Prioritizes specific information like Traffic Light color from Camera or Speed from Radar.

vector<SensorReading> SelfDrivingCar::fuseSensorData(
    const vector<SensorReading>& lidarData,
    const vector<SensorReading>& radarData,
    const vector<SensorReading>& cameraData
)
{
    // Group all readings by their object ID
    map<string, vector<SensorReading>> groupedData;

    for (const auto& r : lidarData)
        groupedData[r.objectID].push_back(r);

    for (const auto& r: radarData)
        groupedData[r.objectID].push_back(r);

    for (const auto& r : cameraData)
        groupedData[r.objectID].push_back(r);

    vector<SensorReading> finalResults;

    // Process each group to create a single fused reading

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

        // Only include objects that meet the confidence threshold or are Bikes (high priority)
        
        if (merged.confidence >= this->minConfidence || sawBike)
            finalResults.push_back(merged);
    }

    return finalResults;
}

// Main logic for Autonomous Navigation.
// 1. Gathers sensor data.
// 2. Fuses sensor data.
// 3. Checks if current GPS target is reached.
// 4. Calculates direction to next target.
// 5. Analyzes obstacles for collision avoidance.
// 6. Adjusts speed (Accelerate/Decelerate/Stop).

void SelfDrivingCar::syncNavigationSystem() {
   if (world == nullptr) return;
 
    // Gather Raw Data

    const vector<WorldObjects*> objects = world->getObjects();
    vector<SensorReading> lidarData = lidar->getReadings(objects, pos, direction);
    vector<SensorReading> radarData = radar->getReadings(objects, pos, direction);
    vector<SensorReading> cameraData = camera->getReadings(objects, pos, direction);

    vector<SensorReading> currentObstacles = fuseSensorData(lidarData, radarData, cameraData);

    // Target Management
    
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

    // Pathfinding / Steering
    
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

    // Obstacle Avoidance Logic
    
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

    // Speed Control Execution
    
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

// Physically updates the car's position in the world based on its speed and direction.

void SelfDrivingCar::executeMovement() {
    move();
}

// Update loop for the car. Calls syncing method and moves if speed > 0.

void SelfDrivingCar::update() {
   syncNavigationSystem();
   if (speed > 0) {
        move();
        simLog << "[SDC] Moved to (" << pos.x << ", " << pos.y << ")" << endl;
    }
}

// Returns true if all GPS targets have been visited.

bool SelfDrivingCar::hasReachedDestination() const{
    return currentTargetIndex >= (int) gpsTargets.size();
}

// Returns the string representation of the current speed state.

string SelfDrivingCar::getStatus() const {
    switch(speedState) {
        case STOPPED: return "STOPPED";
        case HALF_SPEED: return "HALF_SPEED";
        case FULL_SPEED: return "FULL_SPEED";
        default: return "UNKNOWN";
    }
}