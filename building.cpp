// environment.cpp
// Author: Jaden Hicks

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

#include "building.h"

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
    int newCur = std::stoi(cur);
    char newDir = dir[0];
    int newNumPeople = std::stoi(numP);
    if (newCur != current || newDir != direction) { // elevator moved or changing direction
        if (newCur == current+1 || newCur == current-1) { // elevator moved one floor

        } else { // elevator moved multiple floors

        }
        int diffPeople = newNumPeople - numPeople;

    } else if () {

    }
    if (newNumPeople < numPeople) { // check if people got off elevator
        remainingCapacity += numPeople - newNumPeople; // increase remaining capacity
        if (remainingCapacity > capacity) {
            std::cout << "ERROR: elevator remaining capacity greater than capacity! Should not be possible." << std::endl;
            remainingCapacity = capacity;
        }
    }
    numPeople = newNumPeople;
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
