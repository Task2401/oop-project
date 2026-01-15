#ifndef COMMON_H
#define COMMON_H

#include <fstream>

// Represents a coordinate on the 2D grid
 
struct Position {
    int x;
    int y;
};

// Global log file stream
 
extern std::ofstream simLog;

#endif
