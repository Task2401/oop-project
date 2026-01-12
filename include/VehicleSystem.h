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

    public:
        SelfDrivingCar(int startX, int startY, const GridWorld* worldRef, const SimSettings& settings);
        ~SelfDrivingCar();

        void accelerate();
        void decelerate();

        void turn(Direction newDirection);
        
        void collectSensorData();
        void syncNavigationSystem();
        void executeMovement();

        virtual void update() override;

        std::string getStatus() const;
};

#endif