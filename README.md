# Autonomous Vehicle Simulation (C++ / OOP)

![Language](https://img.shields.io/badge/language-C%2B%2B11-blue.svg)
![Build](https://img.shields.io/badge/build-Make-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

A modular simulation of a self-driving car navigating a dynamic grid world. This project demonstrates advanced Object-Oriented Programming principles, Sensor Fusion algorithms, and autonomous decision-making logic.

## Overview
The simulation features an autonomous agent (@) that navigates a 2D environment populated with dynamic traffic (C, B), static obstacles (P), and traffic signaling (R, G, Y, S). The vehicle utilizes a multi-sensor array to perceive its surroundings and employs a weighted fusion algorithm to make real-time navigation decisions.

## Key Technical Features

### 1. Sensor Fusion Engine
The vehicle aggregates data from three distinct sensors to build a unified world model (implementation in VehicleSystem.cpp):
* **Lidar:** 360-degree short-range detection with high precision.
* **Radar:** Long-range forward detection specialized for moving targets.
* **Camera:** Object classification (Traffic Lights/Signs) within a specific Field of View (FOV).
* **Noise Simulation:** Sensors include a stochastic noise model (applyNoise) to simulate real-world imperfections.

### 2. Autonomous Navigation Logic
* **Pathfinding:** Manhattan-distance routing to sequential GPS waypoints.
* **Obstacle Avoidance:** Dynamic speed adjustment (Full/Half/Stop) based on object proximity.
* **Safety Protocols:**
    * **VRU Protection:** Priority tracking for Vulnerable Road Users (Bicycles).
    * **Traffic Compliance:** Automatic adherence to Traffic Lights and STOP signs.

### 3. Architecture (OOP)
* **Polymorphism:** Abstract base class WorldObjects with specialized derived classes (MovingObject, StaticObject).
* **Encapsulation:** Strict separation between the GridWorld environment and the VehicleSystem logic.
* **Memory Management:** RAII principles and proper destructor chaining to ensure zero memory leaks.

## Installation & Usage

### Prerequisites
* G++ Compiler (supporting C++11 standard)
* GNU Make

### Compilation
Build the project using the provided Makefile:

**Linux / macOS:**
```bash
make
```
**Windows (PowerShell / CMD):**
```dos
make
# Or if using MinGW specifically:
mingw32-make
```


### Execution

Run the simulation with custom parameters (dimensions, traffic density, GPS targets):

**Linux / macOS:**
```bash
./avs --dimX 20 --dimY 20 --gps 5 5 15 15 --simulationTicks 200
```
**Windows (PowerShell / CMD):**
```dos
.\avs.exe --dimX 20 --dimY 20 --gps 5 5 15 15 --simulationTicks 200
```


*Use `--help` to see all available configuration flags*

### Visualization

```plaintext
--- POV MAP (Radius: 5) ---
. . . C . . .
. . P . . . .
. . . @ . . .  <-- Autonomous Vehicle
. . . . . . .
. . S . . . .
---------------------------
[SDC] Moved to (10, 11)
[AUTOPILOT] Obstacle detected (CAR)! Stopping.
```

- **@:** Autonomous Car
- **C:** Traffic Car
- **Bike:** Bike
- **R/G/Y:** Traffic Light
- **S:** Stop Sign

### Author
**Anastasios - Christos Kyrios** *Software Engineer / Computer Science Student*

---
*This project was developed as an academic showcase of C++ Software Engineering capabilities.*