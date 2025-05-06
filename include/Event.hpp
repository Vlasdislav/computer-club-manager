#pragma once

#include <string>
#include "Time.hpp"

enum class EventType {
    CLIENT_ARRIVED = 1,
    CLIENT_SAT = 2,
    CLIENT_WAITING = 3,
    CLIENT_LEFT = 4,
    CLIENT_FORCE_LEFT = 11,
    CLIENT_SAT_FROM_QUEUE = 12,
    ERROR = 13
};

class Event {
public:
    Event(Time time, EventType type, std::string clientName, int tableNumber = -1);
    
    static Event createError(Time time, std::string errorMsg);
    
    Time getTime() const;
    EventType getType() const;
    std::string getClientName() const;
    int getTableNumber() const;
    std::string getErrorMsg() const;
    
    std::string toString() const;
    
    static Event parse(const std::string& line);
    
private:
    Time time;
    EventType type;
    std::string clientName;
    int tableNumber;
    std::string errorMsg;
    
    Event(Time time, EventType type, std::string clientName, int tableNumber, std::string errorMsg);
};
