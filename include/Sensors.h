#ifndef SENSORS_H
#define SENSORS_H

#include <vector>
#include <string>

#include "Common.h"
#include "WorldObjects.h"
 
// Structure containing data returned by a sensor for a specific object

struct SensorReading{
    std::string objectID;
    std:: string type;
    Position pos;
    double distance;
    double confidence;
    int speed;
    Direction direction;
    std::string signText;
    LightState lightState;
};
 
// Helper to create a default reading
 
SensorReading createEmptyReading();

// Abstract base class for all sensors
 
class Sensor {
    protected:
        std::string id;

        double baseAccuracy;

        double calculateDistance(Position pos1, Position pos2) const;

        double applyNoise(double conf) const;

    public:
        Sensor(const std::string& sensorID, double accuracy);

        virtual ~Sensor();

        // Pure virtual function to get readings from the environment
 
        virtual std::vector<SensorReading> getReadings(const std::vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) = 0;

        std::string getId() const;
};
 
// Lidar Sensor: Accurate short-range 360 detection

class Lidar : public Sensor {
    public:
        Lidar(const std::string& sensorID);
        virtual ~Lidar();
        
        virtual std::vector<SensorReading> getReadings(const std::vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) override;     
};
 
// Radar Sensor: Detects moving objects at longer range

class Radar : public Sensor {
    public:
        Radar(const std::string& sensorID);
        virtual ~Radar();

        virtual std::vector<SensorReading> getReadings(const std::vector <WorldObjects*>& allObjects, Position carPos, Direction carDir) override;
};

// Camera Sensor: Identifies object types/states (signs, lights) in FOV
 
class Camera : public Sensor {
    public:
        Camera(const std::string& sensorID);
        virtual ~Camera();

        virtual std::vector<SensorReading> getReadings(const std::vector <WorldObjects*>& allObjects, Position carPos, Direction carDir) override;
};

#endif