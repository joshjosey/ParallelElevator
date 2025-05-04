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
Name: setup_building
Author: Jaden Hicks
Purpose: This function reads in an input file and parses it to fill in a 
         building object with a vector of Elevator objects.
Parameters: in - a input stream to the building data file
Returns: A Building object
-----------------------------------------------------------------------------
*/
Building setup_building(std::ifstream& in) {
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
            std::cerr << "Bad line\n";
            return Building(std::vector<Elevator>());
        }
        // save elevator
        e.emplace_back(name, std::stoi(lowest), std::stoi(highest), std::stoi(current), std::stoi(capacity));
    }
    // save building
    return Building(std::move(e));
}
