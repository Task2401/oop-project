#ifndef VEHICLE_SYSTEM_H
#define VEHICLE_SYSTEM_H

#include "WorldObjects.h"
#include "Sensors.h"

enum SpeedState {STOPPED, HALF_SPEED, FULL_SPEED};

class SelfDrivingCar : public MovingObject {
    private:
        SpeedState speedState;

    public:
        SelfDrivingCar(int startX, int startY);

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