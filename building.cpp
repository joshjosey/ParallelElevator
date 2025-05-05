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
Name: getDirFromStr
Author: Jaden Hicks
Purpose: Converts elevator direction string to Dir enum.
Parameters: str - direction string
Returns: Dir
-----------------------------------------------------------------------------
*/
// Dir getDirFromStr(const std::string str) {
//     std::cout << "DEBUG: Received input = [" << str << "]" << std::endl;
//     if (str[0] == 'S')
//         return S;
//     else if (str[0] == 'U')
//         return U;
//     else if (str[0] == 'D')
//         return D;
//     else {
//         std::cout << "ERROR: Invalid Dir: " << str << std::endl;
//         return E;
//     }
// }

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
// int Elevator::updateStatus(const std::string status) {
//     std::istringstream iss(status);
//     std::string elevatorId, current, dir, numPeople, remainingCapacity;
//     if (!std::getline(iss, elevatorId, '|') ||
//         !std::getline(iss, current, '|') ||
//         !std::getline(iss, dir, '|') ||
//         !std::getline(iss, numPeople, '|') ||
//         !std::getline(iss, remainingCapacity, '|')) {
//         std::cout << "ERROR: Invalid status: " << status << "\n";
//         return 1;
//     }
//     std::cout << dir << std::endl;
//     current = std::stoi(current);
//     direction = getDirFromStr(dir);
//     std::cout << direction << std::endl;
//     numPeople = std::stoi(numPeople);
//     if (direction = E)
//         return 1;
//     return 0;
// }

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
