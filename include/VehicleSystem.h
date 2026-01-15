#ifndef VEHICLE_SYSTEM_H
#define VEHICLE_SYSTEM_H

#include "WorldObjects.h"
#include "Sensors.h"

enum SpeedState {STOPPED, HALF_SPEED, FULL_SPEED};

struct SimSettings;
class GridWorld;

class SelfDrivingCar : public MovingObject {
    private:
        SpeedState speedState;
        const GridWorld* world;
        double minConfidence;
        Lidar* lidar;
        Radar* radar;
        Camera* camera;
        std::vector<Position> gpsTargets;
        int currentTargetIndex;

    public:
        SelfDrivingCar(int startX, int startY, const GridWorld* worldRef, const SimSettings& settings);

        ~SelfDrivingCar();

        void accelerate();

        void decelerate();

        void turn(Direction newDirection);
        
        std::vector<SensorReading> fuseSensorData(
            const std::vector<SensorReading>& lidarData, 
            const std::vector<SensorReading>& radarData, 
            const std::vector<SensorReading>& cameraData
        );
        
        void syncNavigationSystem();
        
        void executeMovement();

        virtual void update() override;

        std::string getStatus() const;

        bool hasReachedDestination() const;
};

#endif