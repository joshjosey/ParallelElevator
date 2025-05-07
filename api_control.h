/*
=============================================================================
Title : api_control.h
Description : A header file for api_control.cpp which defines the
              responseBuffer and Person classes
Author : Josh Josey
Date : 05/04/2025
Version : 1.0
=============================================================================
*/
#ifndef API_H
#define API_H

#include <string>
#define MAX_RESPONSE_SIZE 1024

//classes
class ResponseBuffer {
//Purpose: This class is used to store the response from the API.
//Author: Eric Rees
public:
    ResponseBuffer() {
        buffer.reserve(MAX_RESPONSE_SIZE);
    }

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        ResponseBuffer* self = static_cast<ResponseBuffer*>(userp);
        self->buffer.append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    const std::string& str() const { return buffer; }

private:
    std::string buffer;
};


class Person{
//Purpose: This class is used to store the person data returned from the API.
//Author: Josh Josey
public:
    Person() : id(""), start(-1), end(-1), onElevator(false) {}
    Person(const std::string id, int start, int end) : id(id), start(start), end(end), onElevator(false) {}
    /*add more classes here as needed*/
    void print() {
        std::cout << id << " | start = " << start << " | end = " << end << std::endl;
    }
    std::string getId() const { return id; }
    int getStart() const { return start;  }
    int getEnd() const { return end; }
    bool getOnElevator() const { return onElevator; }

private:
    std::string id;
    int start;
    int end;
    bool onElevator;
};


//function prototypes
int simStatus(std::string host);
void simulationControl(std::string host, const std::string command);
std::string elevatorStatus(std::string host, const std::string elevator_id);
Person nextInput(std::string host);
void addToElevator(std::string host, std::string person_id, std::string elevator_id);


#endif