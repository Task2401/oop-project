#ifndef SENSORS_H
#define SENSORS_H

#include <vector>
#include <string>

#include "Common.h"
#include "WorldObjects.h"

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

SensorReading createEmptyReading();

class Sensor {
    protected:
        std::string id;
        double baseAccuracy;

        double calculateDistance(Position pos1, Position pos2) const;
        double applyNoise(double conf) const;

    public:
        Sensor(const std::string& sensorID, double accuracy);
        virtual ~Sensor();

        virtual std::vector<SensorReading> getReadings(const std::vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) = 0;

        std::string& getId() const;
};

class Lidar : public Sensor {
    public:
        Lidar(const std::string& sensorID);
        virtual std::vector<SensorReading> getReadings(const std::vector<WorldObjects*>& allObjects, Position carPos, Direction carDir) override;     
};

class Radar : public Sensor {
    public:
        Radar(const std::string sensorID);
        virtual std::vector<SensorReading> getReadings(const std::vector <WorldObjects*>& allObjects, Position carPos, Direction carDir) override;
};

class Camera : public Sensor {
    public:
        Camera(const std::string sensorID);
        virtual std::vector<SensorReading> getReadings(const std::vector <WorldObjects*>& allObjects, Position carPos, Direction carDir) override;
};

#endif