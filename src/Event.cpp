#include "Event.hpp"
#include <sstream>

Event::Event(Time time, EventType type, std::string clientName, int tableNumber)
    : time(time), type(type), clientName(std::move(clientName)), 
      tableNumber(tableNumber), errorMsg("") {}

Event Event::createError(Time time, std::string errorMsg) {
    return Event(time, EventType::ERROR, "", -1, std::move(errorMsg));
}

Event::Event(Time time, EventType type, std::string clientName, 
        int tableNumber, std::string errorMsg)
    : time(time), type(type), clientName(std::move(clientName)),
    tableNumber(tableNumber), errorMsg(std::move(errorMsg)) {}

Time Event::getTime() const { return time; }
EventType Event::getType() const { return type; }
std::string Event::getClientName() const { return clientName; }
int Event::getTableNumber() const { return tableNumber; }
std::string Event::getErrorMsg() const { return errorMsg; }

std::string Event::toString() const {
    std::ostringstream oss;
    oss << time.toString() << " " << static_cast<int>(type);
    
    switch (type) {
        case EventType::CLIENT_ARRIVED:
        case EventType::CLIENT_WAITING:
        case EventType::CLIENT_LEFT:
        case EventType::CLIENT_FORCE_LEFT:
            oss << " " << clientName;
            break;
        case EventType::CLIENT_SAT:
        case EventType::CLIENT_SAT_FROM_QUEUE:
            oss << " " << clientName << " " << tableNumber;
            break;
        case EventType::ERROR:
            if (!errorMsg.empty()) {
                oss << " " << errorMsg;
            }
            break;
    }
    
    return oss.str();
}

Event Event::parse(const std::string& line) {
    std::istringstream iss(line);
    std::string timeStr, typeStr;
    iss >> timeStr >> typeStr;
    
    Time time(timeStr);
    EventType type = static_cast<EventType>(std::stoi(typeStr));
    
    if (type == EventType::ERROR) {
        std::string errorMsg;
        std::getline(iss >> std::ws, errorMsg);
        return createError(time, errorMsg);
    }
    
    std::string clientName;
    iss >> clientName;
    
    if (type == EventType::CLIENT_SAT || type == EventType::CLIENT_SAT_FROM_QUEUE) {
        int tableNumber;
        iss >> tableNumber;
        return Event(time, type, clientName, tableNumber);
    }
    
    return Event(time, type, clientName);
}
