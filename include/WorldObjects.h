#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <iostream>
#include <vector>
#include <string>

#include "Common.h"

 
// Enum for cardinal directions
 
enum Direction {NORTH, SOUTH, EAST, WEST};
 
// Enum for Traffic Light colors

enum LightState {RED, GREEN, YELLOW};

// WorldObjects -> Abstract base class for all entities in the grid
 
class WorldObjects {
    protected:
        std::string id;
        Position pos;
        char glyph;

    public:
        WorldObjects(const std::string& objectID, int x, int y, char g);
        
        virtual ~WorldObjects();

        virtual void update() = 0;

        const std::string& getId() const;

        Position getPosition() const;

        char getGlyph() const;
};

// StaticObject -> Objects that do not move (Signs, Lights, Parked Cars)
 
class StaticObject : public WorldObjects {
    public:
        StaticObject(const std::string& objectID, int x, int y, char g);
        
        virtual void update() override;
};

// TrafficLight -> Cycles through Red, Green, Yellow states

class TrafficLight : public StaticObject {
    private:
        LightState state;
        int timer;

    public:
        TrafficLight(const std::string& objectID, int x, int y);

        virtual void update() override;

        LightState getState() const;
};

// TrafficSign -> Represents a road sign with text (e.g. "STOP")
 
class TrafficSign : public StaticObject {
    private:
        std::string text;
    
    public:
        TrafficSign(const std::string& objectID, int x, int y, const std::string& txt);

        const std::string& getText() const;
};

// StationaryVehicles -> Represents a parked car (obstacle)
 
class StationaryVehicles : public StaticObject {
    public:
        StationaryVehicles(const std::string& objectID, int x, int y);
};

// MovingObject -> Base class for dynamic objects (Cars, Bikes)
 
class MovingObject : public WorldObjects {
    protected:
        int speed;
        Direction direction;

    public:
        MovingObject(const std::string& objectID, int x, int y, char g, int s, Direction d);

        virtual void update() override;

        void move();

        int getSpeed() const;

        Direction getDirection() const;
};
 
// Car -> Represents other moving cars (traffic)
 
class Car : public MovingObject {
    public:
        Car(const std::string& objectID, int x, int y, Direction d);
};

// Bike -> Represents moving bicycles
 
class Bike : public MovingObject {
    public:
        Bike(const std::string& objectID, int x, int y, Direction d);
};

#endif