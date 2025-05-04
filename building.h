// environment.h
// Author: Jaden Hicks

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

/*
struct Elevator {
    std::string name; // elevatorID
    int lowest;       // lowest floor
    int highest;      // highest floor
    int current;      // current floor
    int capacity;     // capacity

    Elevator(const std::string& name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), capacity(capacity) {}
};
*/

class Elevator {
//Purpose: This class is used to store the elevator data returned from the API.
//Author: Jaden Hicks & Josh Josey
public:
    Elevator() : name(""), lowest(-1), highest(-1), current(-1), capacity(-1) {}

    Elevator(const std::string &name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), capacity(capacity) {}

    void print(){
        std::cout << name << " | Lowest = " << lowest << " | Highest = " << highest << " | Current = " << current << " | Capactiy " << capacity << std::endl;
    }

    std::string getName() const { return name; }
    int getLowest() const { return lowest; }
    int getHighest() const { return highest; }
    int getCurrent() const { return current; }
    int getCapactiy() const { return capacity; }

private:
    std::string name; // elevatorID
    int lowest;       // lowest floor
    int highest;      // highest floor
    int current;      // current floor
    int capacity;     // capacity
};

/*
struct Building {
    std::vector<Elevator> elevators;

    Building(std::vector<Elevator> elevators)
        : elevators(std::move(elevators)) {}

    int num_elevators() const {
        return static_cast<int>(elevators.size());
    }
};
*/

class Building {
//Purpose: This class is used to store the building data returned from the API.
//Author: Jaden Hicks & Josh Josey
public:
    std::vector<Elevator> elevators;
    //Constructors
    Building() : elevators() {};
    Building(std::vector<Elevator> e) : elevators(move(e)) {};

    int numElevators() const { return static_cast<int>(elevators.size());}
private:
    //std::vector<Elevator> elevators;
};

/* setup_building
    - reads building configuration from inp
    - builds building_t and saves to *b
    - returns 0 if successful, 1 otherwise
*/
Building setup_building(std::ifstream& inp);

#endif