/*
=============================================================================
Title : building.h
Description : A header file for building.cpp which defines the Elevator and
              Building classes.
Author : Jaden Hicks
Date : 05/04/2025
Version : 1.0
=============================================================================
*/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <sstream>
#include <vector>

#include "api_control.h"

class Elevator {
//Purpose: This class is used to store the elevator data returned from the API and track the elevators.
//Author: Jaden Hicks & Josh Josey
public:
    /*Constructors*/
    Elevator() : name(""), lowest(-1), highest(-1), current(-1), direction('S'), numPeople(0), remainingCapacity(-1), capacity(-1) {}

    Elevator(const std::string &name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), direction('S'), numPeople(0), remainingCapacity(capacity), capacity(capacity) {}

    /*Getter function prototypes*/
    std::string getName() const;
    int getLowestFloor() const;
    int getHighestFloor() const;
    int getCurrentFloor() const;
    char getDirection() const;
    int getNumPeople() const;
    int getRemainingCapacity() const;
    int getMaxCapacity() const;

    /*Elevator functions prototypes*/
    int updateStatus(const std::string status);
    bool inRange(int start, int end);
    bool checkDirection(int floor);
    bool empty();
    void print();

private:
    std::string name;       // elevatorID
    int lowest;             // lowest floor
    int highest;            // highest floor
    int current;            // current floor
    char direction;         // direction
    int numPeople;          // number of people in elevator
    int remainingCapacity;  // remaining capacity
    int capacity;           // capacity
};

class Building {
//Purpose: This class is used to store the building data returned from the API.
//Author: Jaden Hicks & Josh Josey
public:
    std::vector<Elevator> elevators;
    //Constructors
    Building() : elevators() {};
    Building(std::vector<Elevator> e) : elevators(move(e)) {};

    int numElevators() const { return static_cast<int>(elevators.size());}
};

// Purpose: reads building configuration from inp, builds building_t
// Returns: full building on success, empty building on failure
Building setupBuilding(std::ifstream& inp);

#endif