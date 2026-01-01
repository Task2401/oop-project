#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <iostream>
#include <vector>
#include <string>

enum Direction {NORTH, SOUTH, EAST, WEST};
enum LightState {RED, GREEN, YELLOW};

struct Position{
    int x;
    int y;
};

class WorldObjects {
    protected:
        std::string id;
        Position pos;
        char glyph;

    public:
        WorldObjects(const std::string& objectID, int x, int y, char g);
        
        virtual ~WorldObjects();

        virtual void update() = 0;

        const std::string& gedId() const;
        Position getPosition() const;
        char getGlyph() const;
};

class StaticObject : public WorldObjects {
    public:
        StaticObject(const std::string& objectID, int x, int y, char g);
        
        virtual void update() override;

};

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

#endif