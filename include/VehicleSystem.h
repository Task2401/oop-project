#ifndef VEHICLE_SYSTEM_H
#define VEHICLE_SYSTEM_H

#include "WorldObjects.h"
#include "Sensors.h"

 
// Enum defining possible speed levels

enum SpeedState {STOPPED, HALF_SPEED, FULL_SPEED};

struct SimSettings;
class GridWorld;
 
// Represents the autonomous vehicle with sensors and navigation logic

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

        // Constructor that initializes car and sensors

        SelfDrivingCar(int startX, int startY, const GridWorld* worldRef, const SimSettings& settings);
 
        // Destructor cleaning up sensor memory
    
        ~SelfDrivingCar();
 
        // Increases speed state

        void accelerate();

        // Decreases speed state

        void decelerate();
 
        // Changes current moving direction
        
        void turn(Direction newDirection);
         
        // Merges data from multiple sensors into a single consistent view
        
        std::vector<SensorReading> fuseSensorData(
            const std::vector<SensorReading>& lidarData, 
            const std::vector<SensorReading>& radarData, 
            const std::vector<SensorReading>& cameraData
        );
         
        // Main logic loop: Sense -> Plan -> Act
        
        void syncNavigationSystem();
         
        // Applies movement updates

        void executeMovement();
         
        // Called every tick to update car state
        
        virtual void update() override;

        std::string getStatus() const;

        bool hasReachedDestination() const;
};

#endif