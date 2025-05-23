/*
=============================================================================
Title : main.cpp
Description : This program communicates with a simulated elevator operating
              system using a Flask API written by Dr. Eric Rees. It
              implements a scheduling algorithm to run elevators efficiently.
Author : Jaden Hicks & Josh Josey
Date : 05/04/2025
Version : 1.0
Usage : Compile using make and run ./scheduler_os once API is running
        Executable takes to command line arguments:
        -   Filepath to building configuration file
        -   Port that API is listening to
Notes : -   This program requires that the API is running on the port provided
            as a command line argument
        -   Must be compiled with the -pthread -lcurl flags
C++ Version : 11 or higher
=============================================================================
*/
#include <iostream>
#include <fstream>
#include <vector>

#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <curl/curl.h>
#include <chrono>
#include <deque>

#include "building.h"
#include "api_control.h"

//Function Prototypes
void inputThread();
void schedulerThread();
void outputThread();
int calculateScore(Elevator e, Person p);

//Global Variables
std::string host;
Building building;
std::deque<Person> people_q;
std::deque<std::pair<std::string,std::string>> output_q; //always push as (person_id, elevator_id)

//Synchonization Variables
std::atomic<bool> sim_complete_flag(false);
std::condition_variable io_cv;
std::condition_variable schedule_cv;
std::mutex mtx;

// Author: Josh Josey & Jaden Hicks
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "ERROR: Not enough arguments\nUsage: " << argv[0] << " <path to input file> <port>" << std::endl;
        return 1;
    }

    // -- Following was adapted from GitHub Copilot --

    std::ifstream inp(argv[1]);
    if (!inp.is_open()) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    building = setupBuilding(inp);
    if (building.numElevators() == 0) {
        std::cerr << "Building setup failed." << std::endl;
        return 1;
    }
    inp.close();

    std::cout << "Building has " << building.numElevators() << " elevators." << std::endl;
    for (int i = 0; i < building.numElevators(); i++) {
        building.elevators[i].print();
    }
    // -- End of GitHub Copilot adaptation -- (this comment was generated by Copilot haha)

    
    std::cout << "Starting Elevator OS..." << std::endl;
    std::string port = argv[2];
    host = "http://localhost:" + port;
    std::cout << "Host: " << host << std::endl;
    //initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    //ensure the api is running
	simStatus(host);
	simulationControl(host,"start");
    if (simStatus(host) < 2) {
        std::cerr << "Simulation failed to start." << std::endl;
        exit(1);
    };

    //start the threads
    std::thread inp_thread(inputThread);
    std::thread schedule_thread(schedulerThread);
    std::thread outp_thread(outputThread);
    inp_thread.join();
    schedule_thread.join();
    outp_thread.join();


    // ensure the API stopped
	simulationControl(host,"stop");
	simStatus(host);

	//cleanup curl
    curl_global_cleanup();
    return 0;
}


void inputThread(){
    int status = simStatus(host);
    while(true){
        //Get the next person waiting
        Person next = nextInput(host);

        //Get people until nobody else is coming
        while(next.getId() != "NONE") {
            //DEBUGnext.print();
            std::cout << "\tINPUT: Adding " << next.getId() << " to the people q " << std::endl;
            people_q.push_back(next);
            next = nextInput(host);
        }

        //Check if the simulation is complete
        status = simStatus(host);
        if(status == 3){
            std::cout << "INPUT: Simulation complete, terminating..." << std::endl;
            sim_complete_flag.store(true);
            schedule_cv.notify_one();
            return;
        }

        //If there is any data then wake the other threads then go to sleep, otherwise wait a second and retry
        if(!people_q.empty()){
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "INPUT: People waiting to board waking the scheduler, going to sleep..." << std::endl;
            schedule_cv.notify_one();
            io_cv.wait(lock, []()
                       { return people_q.empty()  || sim_complete_flag.load(); });
            std::cout << "INPUT: Checking for passengers" << std::endl;

        } else{
            std::cout << "\tINPUT: Nobody waiting to board an elevator, waiting to retry" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void schedulerThread() {
    std::deque<Person> straggler_q;
    while(true){
        //Wait for the other threads to be complete
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "SCHEDULER: Going to sleep..." << std::endl;
        io_cv.notify_all();
        schedule_cv.wait(lock, [](){ 
            return (!people_q.empty() && output_q.empty()) || sim_complete_flag.load(); 
        });

        //Add the stragglers to the people_q
        if(!straggler_q.empty()){
            people_q.insert(people_q.begin(), straggler_q.begin(), straggler_q.end());
            straggler_q.clear();
        }

        //Update the elevators
        std::cout << "SCHEDULER: updating elevators" << std::endl;
        for (auto &e : building.elevators)
        {
            std::string status = elevatorStatus(host, e.getName());
            e.updateStatus(status);
            std::cout << "\tSCHEDULER: ";
            e.print();
        }

        //Assign each person to an elevator
        std::cout << "SCHEDULER: assigning people to elevators" << std::endl;
        while(!people_q.empty()){
            Person p(people_q.front());
            people_q.pop_front();
            int min = 999999;
            int min_idx = -1;
            std::cout << "SCHEDULER: comparing elevators for " << p.getId() << " (" << p.getStart() << " -> " << p.getEnd() << ") " << std::endl;
            for(int i = 0; i < building.numElevators(); i++){
                //check if it is in range and has capactiy
                if ( !building.elevators[i].inRange(p.getStart(), p.getEnd()) ){
                    std::cout << "\tSCHEDULER: " << building.elevators[i].getName() << " cannot reach desired floor" << std::endl;
                    continue;
                }
                if ( building.elevators[i].getNumPeople() == building.elevators[i].getMaxCapacity() ){
                    std::cout << "\tSCHEDULER: " << building.elevators[i].getName() << " has no space" << std::endl;
                    continue;
                }

                //Add together the penalties & rewards to find the score
                int score = calculateScore(building.elevators[i], p);
                std::cout << "\tSCHEDULER: " << building.elevators[i].getName() << " has a score of " << score << std::endl;

                //See if it is the lowest score
                if(score < min){
                    min = score;
                    min_idx = i;
                }
            }
            //Enqueue the best elevator OR requeue the person
            if(min_idx >= 0){
                output_q.emplace_back(p.getId(), building.elevators[min_idx].getName());
                std::cout << "SCHEDULER: Queueing " << output_q.back().first << " ( " << p.getStart() << " -> " << p.getEnd() << ") " 
                                                    << " on elevator " << output_q.back().second << " (" << building.elevators[min_idx].getCurrentFloor()
                                                    << " -> " << building.elevators[min_idx].getDirection() << ") "<< std::endl;
            }
            else
            {
                //Add the people who could not get on an elevator to the straggler queue
                straggler_q.push_back(p);
                std::cout << "SCHEDULER: Could not find an elevator for " << p.getId() << " requeueing & updating elevators" << std::endl;
                for (auto &e : building.elevators)
                {
                    std::string status = elevatorStatus(host, e.getName());
                    e.updateStatus(status);
                }
            }
        }
        //If the simulation is complete, wake the remaining threads and terminate
        if(sim_complete_flag.load() == true ){
            std::cout << "SCHEDULER: Simulation complete, terminating..." << std::endl;
            io_cv.notify_all();
            return;
        }
    }
}

void outputThread() {
    while(true) {
        //Add people to elevators
        while(!output_q.empty()){
            std::string p_id = output_q.front().first;
            std::string e_id = output_q.front().second;
            output_q.pop_front();

            std::cout << "OUTPUT: Placing " << p_id << " on elevator " << e_id << std::endl;
            addToElevator(host, p_id, e_id);
        }
        //Check if the simulation is complete
        if (sim_complete_flag.load() == true) {
            std::cout << "OUTPUT: Simulation complete, terminating..." << std::endl;
            schedule_cv.notify_one();
            return;
        }
        //Wake the other threads then go to sleep
        {
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "OUTPUT: All passengers placed, going to sleep..." << std::endl;
            schedule_cv.notify_one();
            io_cv.wait(lock, [](){ return !output_q.empty() || sim_complete_flag.load(); });
        }
    }
}

/*
Authors: Josh Josey, refactored by copilot
*/
int calculateScore(Elevator e, Person p) {
    int max_range = e.getHighestFloor() - e.getLowestFloor();

    int distance_penalty = abs(e.getCurrentFloor() - p.getStart());

    int start_direction_penalty = !e.checkDirection(p.getStart()) ? (max_range) / 2 : 0;

    int end_direction_penalty = !e.checkDirection(p.getEnd()) ? (max_range) / 2 : 0;

    int passenger_penalty = !e.empty() ? (e.getNumPeople()) * 5 : 0;

    return distance_penalty + start_direction_penalty + end_direction_penalty + passenger_penalty;
}