// environment.h
// Author: Jaden Hicks

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <sstream>
#include <vector>
// #include <mutex>

// enum Dir { S, U, D, E };

// Dir getDirFromStr(const std::string str);

class Elevator {
//Purpose: This class is used to store the elevator data returned from the API and track the elevators.
//Author: Jaden Hicks & Josh Josey
public:
    Elevator() : name(""), lowest(-1), highest(-1), current(-1), capacity(-1) {}

    Elevator(const std::string &name, int lowest, int highest, int current, int capacity)
        : name(name), lowest(lowest), highest(highest), current(current), capacity(capacity) {}

    void print(){
        std::cout << name << " | Lowest = " << lowest << " | Highest = " << highest << " | Current = " << current << " | Direction " << direction << " | NumPeople " << numPeople << " | Capactiy " << capacity << std::endl;
    }

    std::string getName() const { return name; }
    int getLowest() const { return lowest; }
    int getHighest() const { return highest; }
    int getCurrent() const { return current; }
    char getDirection() const { return direction; }
    int getNumPeople() const { return numPeople; }
    int getCapactiy() const { return capacity; }

    int updateStatus(const std::string status) {
        std::istringstream iss(status);
        std::string elevatorId, current, dir, numP, remainingCapacity;
        if (!std::getline(iss, elevatorId, '|') ||
            !std::getline(iss, current, '|') ||
            !std::getline(iss, dir, '|') ||
            !std::getline(iss, numP, '|') ||
            !std::getline(iss, remainingCapacity, '|')) {
            std::cout << "ERROR: Invalid status: " << status << "\n";
            return 1;
        }
        current = std::stoi(current);
        direction = dir[0];
        std::cout << numP << std::endl;
        numPeople = std::stoi(numP);
        std::cout << numPeople << std::endl;
        if (direction == 'E')
            return 1;
        return 0;
    }

private:
    std::string name; // elevatorID
    int lowest;       // lowest floor
    int highest;      // highest floor
    int current;      // current floor
    char direction;    // direction
    int numPeople;    // number of people in elevator
    int capacity;     // capacity

    // std::mutex lock;
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
private:
    //std::vector<Elevator> elevators;
};

/* setupBuilding
    - reads building configuration from inp
    - builds building_t and saves to *b
    - returns 0 if successful, 1 otherwise
*/
Building setupBuilding(std::ifstream& inp);

#endif