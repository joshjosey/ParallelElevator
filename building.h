// environment.h
// Author: Jaden Hicks

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
// #include <mutex>

enum Dir { S, U, D, E };

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
    Dir getDirection() const { return direction; }
    int getNumPeople() const { return numPeople; }
    int getCapactiy() const { return capacity; }

    int updateStatus(const std::string status);

private:
    std::string name; // elevatorID
    int lowest;       // lowest floor
    int highest;      // highest floor
    int current;      // current floor
    Dir direction;    // direction
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