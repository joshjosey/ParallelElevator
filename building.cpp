/*
=============================================================================
Title : building.cpp
Description : Implements functions for elevator and building classes.
Author : Jaden Hicks
Date : 05/04/2025
Version : 1.0
=============================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "building.h"

/*Get functions for the elevator*/
std::string Elevator::getName() const { return this->name; }
int Elevator::getLowestFloor() const { return this->lowest; }
int Elevator::getHighestFloor() const { return this->highest; }
int Elevator::getCurrentFloor() const { return this->current; }
char Elevator::getDirection() const { return this->direction; }
int Elevator::getNumPeople() const { return numPeople; }
int Elevator::getRemainingCapacity() const { return remainingCapacity; }
int Elevator::getMaxCapacity() const { return capacity; }


bool Elevator::empty() { return numPeople == 0; }
void Elevator::print(){
    std::cout << name << " | Lowest = " << lowest << " | Highest = " << highest << " | Current = " << current
              << " | Direction " << direction << " | NumPeople " << numPeople << " | RemainingCapacitiy "
              << remainingCapacity << " | Capacity " << capacity << std::endl;
}
/*
-----------------------------------------------------------------------------
Name: updateStatus
Author: Jaden Hicks
Purpose: This function extracts status info from API response and updates
         elevator variables.
Parameters: status - string response from ElevatorStatus call
Returns: 0 = success;  1 = failure
-----------------------------------------------------------------------------
*/
int Elevator::updateStatus(const std::string status) {
    std::istringstream iss(status);
    std::string elevatorId, cur, dir, numP, remainingCap;
    if (!std::getline(iss, elevatorId, '|') ||
        !std::getline(iss, cur, '|') ||
        !std::getline(iss, dir, '|') ||
        !std::getline(iss, numP, '|') ||
        !std::getline(iss, remainingCap, '|')) {
        std::cout << "ERROR: Invalid status: " << status << "\n";
        return 1;
    }
    current = std::stoi(cur);
    direction = dir[0];
    numPeople = std::stoi(numP);
    remainingCapacity = std::stoi(remainingCap);
    if (direction == 'E')
        return 1;
    return 0;
}

/*
-----------------------------------------------------------------------------
Name: inRange
Author: Josh Josey & Jaden Hicks
Purpose: This function checks if start and end floors are in range for the
         elevator.
Parameters: (int) start, (int) end
Returns: (bool) true if in range, false if out of range
-----------------------------------------------------------------------------
*/
bool Elevator::inRange(int start, int end){
    //if the person is starting in bounds
    if ((start >= this->lowest) && (start <= this->highest) && (end >= this->lowest) && (end <= this->highest)){
        return true;
    }
    return false;
}

/*
-----------------------------------------------------------------------------
Name: inRange
Author: Josh Josey
Purpose: This function checks if the elevator is moving toward a floor
Parameters: (int) floor
Returns: (bool) true if in range, false if out of range
-----------------------------------------------------------------------------
*/
bool Elevator::checkDirection(int floor){
    //if the elevator is stationary and empty always return true
    if (this->empty() && this->direction == 'S'){
        return true;
    }
    //if the elevator is unloading on this floor
    if(this->current == floor && this->direction == 'S'){
        return true;
    }
    //if the floor is below the current and the elevator is moving down
    if(this->current > floor && this->direction == 'D'){
        return true;
    }
    //if the floor is above the current and the elevator is moving up
    if(this->current < floor && this->direction == 'U'){
        return true;
    }
    return false;
}

/*
-----------------------------------------------------------------------------
Name: setupBuilding
Author: Jaden Hicks
Purpose: This function reads in an input file and parses it to fill in a 
         building object with a vector of Elevator objects.
Parameters: in - a input stream to the building data file
Returns: A Building object
-----------------------------------------------------------------------------
*/
Building setupBuilding(std::ifstream& in) {
    // read elevators
    std::vector<Elevator> e;
    std::string line;
    while(std::getline(in, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
            break;
        
        std::istringstream iss(line);
        std::string name, lowest, highest, current, capacity;
        if (!std::getline(iss, name, '\t') ||
            !std::getline(iss, lowest, '\t') ||
            !std::getline(iss, highest, '\t') ||
            !std::getline(iss, current, '\t') ||
            !std::getline(iss, capacity, '\t')) {
            std::cout << "ERROR: Bad line: " << line << std::endl;
            return Building(std::vector<Elevator>());
        }
        // save elevator
        e.emplace_back(name, std::stoi(lowest), std::stoi(highest), std::stoi(current), std::stoi(capacity));
    }
    // save building
    return Building(std::move(e));
}
