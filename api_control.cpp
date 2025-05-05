/*
=============================================================================
Title : api_control.cpp
Description : This function communicates with Dr. Eric Rees' API for 
			  elevator control. It is service functions used in the
			  main code.
Author : Josh Josey
Date : 05/04/2025
Version : 1.0
Usage : Compile and run this program using the GNU C++ compiler
Notes : This program requires that the API is running on the port provided
        as a parameter to the functions
C++ Version : 11 or higher
=============================================================================
*/
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include "api_control.h"


/*
-----------------------------------------------------------------------------
Name: simulationStatus
Author: Josh Josey
Purpose: This function sends either a GET request to check the simulation
		 status
Parameters: host - the host and port of the API
Returns: An integer where 1 = simulation is not runnning
						  2 = simulation currently running
						  3 = simulation complete
-----------------------------------------------------------------------------
*/
int simStatus(std::string host){
    CURL* curl = curl_easy_init();
	std::string url = host + "/Simulation/check";
	int status = -1;
	if (curl)
	{
		ResponseBuffer response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseBuffer::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        curl_easy_perform(curl);
		std::cout << url << "\tGET -> " << response.str() << std::endl;

        curl_easy_cleanup(curl);
		
		if (response.str() == "Simulation is not running.") {
			status = 1;
		} else if (response.str() == "Simulation is already running.") {
			status = 2;
		} else if (response.str() == "Simulation is complete.") {
			status = 3;
		}
		

	}
	return status;
}

/*
-----------------------------------------------------------------------------
Name: simulationControl
Author: Josh Josey
Purpose: This function sends  a PUT request to change the simulation status
Parameters: host - the host and port of the API
            command - "start" = start the simulation (PUT)
					  "stop" = stop the simulation (PUT) 
Returns: None
-----------------------------------------------------------------------------
*/
void simulationControl(std::string host, const std::string command){
    CURL* curl = curl_easy_init();
	std::string url = host + "/Simulation/" + command;
	if (curl)
	{
		ResponseBuffer response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseBuffer::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        if(command == "start" || command == "stop"){
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        }else{
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        }

        curl_easy_perform(curl);

		if(command == "start" || command == "stop"){
			std::cout << url << "\tPUT -> " << response.str() << std::endl;
		}else{
			std::cout << url << "\tGET -> " << response.str() << std::endl;
		}

        curl_easy_cleanup(curl);
	}
}


/*
-----------------------------------------------------------------------------
Name: elevatorStatus
Author: Josh Josey
Purpose: This function sends either GET request to check and print the 
		 status of a specific elevator
Parameters: host - the host and port of the API
            elevator_id - the unique name of the elevator
Returns: None
-----------------------------------------------------------------------------
*/
void elevatorStatus(std::string host, const std::string elevator_id){
    CURL* curl = curl_easy_init();
	std::string url = host + "/ElevatorStatus/" + elevator_id;
	if (curl)
	{
		ResponseBuffer response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseBuffer::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");


        curl_easy_perform(curl);

        std::cout << url << "\tGET -> " << response.str() << std::endl;

        curl_easy_cleanup(curl);
	}
}

/*
-----------------------------------------------------------------------------
Name: nextInput
Author: Josh Josey
Purpose: This function sends a GET request to the API to retrieve the next
		 person in the input file
Parameters: host - the host and port of the API
Returns: A person object initialized with the API data
-----------------------------------------------------------------------------
*/
Person nextInput(std::string host){
	CURL* curl = curl_easy_init();
	std::string url = host + "/NextInput";
	std::string id = "NONE";
	std::string start_floor = "-1";
	std::string end_floor = "-1";
	if (curl)
	{
		ResponseBuffer response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseBuffer::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");


        curl_easy_perform(curl);

        std::cout << url << "\tGET -> " << response.str() << std::endl;
		
		//The resoonse should always be ID Start End Timestamp
		if(response.str() != "NONE"){
			/*Copilot provided splitting function */
			std::istringstream res(response.str());
			std::getline(res, id, '|');
			std::getline(res, start_floor, '|');
			std::getline(res, end_floor);
		}
		curl_easy_cleanup(curl);
	}
	return Person(id, stoi(start_floor), stoi(end_floor));
}

/*
-----------------------------------------------------------------------------
Name: addToElevator
Author: Josh Josey
Purpose: This function sends a PUT request to the API to add a person to an
		 elevator using their ID's
Parameters: host - the host and port of the API
			person_id = the unique ID of the person to be added
			elevator_id = the ID of the elevator to add the person to
Returns: None
-----------------------------------------------------------------------------
*/
void addToElevator(std::string host, std::string person_id, std::string elevator_id){
    CURL* curl = curl_easy_init();
	std::string url = host + "/AddPersonToElevator/" + person_id + "/" + elevator_id;
	if (curl)
	{
		ResponseBuffer response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseBuffer::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        curl_easy_perform(curl);
        std::cout << url << "\tPUT -> " << response.str() << std::endl;

        curl_easy_cleanup(curl);
	}
}
