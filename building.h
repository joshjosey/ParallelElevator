// environment.h
// Author: Jaden Hicks

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
    Elevator() : name(""), lowest(-1), highest(-1), current(-1), direction('S'), numPeople(0), remainingCapacity(-1), capacity(-1) {}

    Elevator(const std::string &name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), direction('S'), numPeople(0), remainingCapacity(capacity), capacity(capacity) {}

    void print(){
        std::cout << name << " | Lowest = " << lowest << " | Highest = " << highest << " | Current = " << current
                  << " | Direction " << direction << " | NumPeople " << numPeople << " | RemainingCapacitiy "
                  << remainingCapacity << " | Capacity " << capacity << std::endl;
    }

    std::string getName() const { return name; }
    int getLowest() const { return lowest; }
    int getHighest() const { return highest; }
    int getCurrent() const { return current; }
    char getDirection() const { return direction; }
    int getNumPeople() const { return numPeople; }
    int getRemainingCapacity() const { return remainingCapacity; }
    int getCapacity() const { return capacity; }

    void addPerson(Person p) { people.emplace_back(p); }
    int updateStatus(const std::string status);
    void decrementRemainingCapacity() { if (remainingCapacity > 0) remainingCapacity--; }
    bool inRange(int start, int end);

private:
    std::string name;       // elevatorID
    int lowest;             // lowest floor
    int highest;            // highest floor
    int current;            // current floor
    char direction;         // direction
    int numPeople;          // number of people in elevator
    std::vector<Person> people;
    int remainingCapacity;  // local tracker of remaining capacity, including people already on elevator and assigned to it (not updated by API)
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