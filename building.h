// environment.h
// Author: Jaden Hicks

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

struct Elevator {
    std::string name; // elevatorID
    int lowest;       // lowest floor
    int highest;      // highest floor
    int current;      // current floor
    int capacity;     // capacity

    Elevator(const std::string& name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), capacity(capacity) {}
};

struct Building {
    std::vector<Elevator> elevators;

    Building(std::vector<Elevator> elevators)
        : elevators(std::move(elevators)) {}

    int num_elevators() const {
        return static_cast<int>(elevators.size());
    }
};

/* setup_building
    - reads building configuration from inp
    - builds building_t and saves to *b
    - returns 0 if successful, 1 otherwise
*/
Building setup_building(std::ifstream& inp);

#endif