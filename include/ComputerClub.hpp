#pragma once

#include <vector>
#include <string>
#include <map>
#include <queue>
#include "Time.hpp"
#include "Event.hpp"

struct Table {
    int number;
    int revenue = 0;
    Time busyTime;
    std::string currentClient;
    Time startTime;
};

class ComputerClub {
public:
    ComputerClub(int tablesCount, Time openingTime, Time closingTime, int hourCost);
    
    void processEvent(const Event& event);
    void endOfDay();
    
    std::vector<std::string> getOutput() const;
    
private:
    int tablesCount;
    Time openingTime;
    Time closingTime;
    int hourCost;
    
    std::vector<Table> tables;
    std::map<std::string, int> clients;
    std::queue<std::string> waitingQueue;
    std::vector<std::string> output;
    
    void addOutputEvent(const Event& event);
    void addError(const Time& time, const std::string& error);
    
    void clientArrived(const Event& event);
    void clientSat(const Event& event);
    void clientWaiting(const Event& event);
    void clientLeft(const Event& event);
    
    void freeTable(int tableNumber, const Time& currentTime);
    void sitClient(const std::string& clientName, int tableNumber, const Time& currentTime);
    int calculateRevenue(const Time& startTime, const Time& endTime) const;
};
