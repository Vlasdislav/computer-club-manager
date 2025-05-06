#include "ComputerClub.hpp"

ComputerClub::ComputerClub(int tablesCount, Time openingTime, Time closingTime, int hourCost)
    : tablesCount(tablesCount), openingTime(openingTime), closingTime(closingTime), hourCost(hourCost) {
    tables.resize(tablesCount);
    for (int i = 0; i < tablesCount; ++i) {
        tables[i].number = i + 1;
    }
}

void ComputerClub::processEvent(const Event& event) {
    addOutputEvent(event);
    
    switch (event.getType()) {
        case EventType::CLIENT_ARRIVED:
            clientArrived(event);
            break;
        case EventType::CLIENT_SAT:
            clientSat(event);
            break;
        case EventType::CLIENT_WAITING:
            clientWaiting(event);
            break;
        case EventType::CLIENT_LEFT:
            clientLeft(event);
            break;
        default:
            break;
    }
}

void ComputerClub::endOfDay() {
    std::vector<std::string> remainingClients;
    for (const auto& client : clients) {
        remainingClients.push_back(client.first);
    }
    std::sort(remainingClients.begin(), remainingClients.end());
    
    for (const auto& client : remainingClients) {
        Event event(closingTime, EventType::CLIENT_FORCE_LEFT, client);
        addOutputEvent(event);
        
        auto it = clients.find(client);
        if (it != clients.end()) {
            freeTable(it->second, closingTime);
            clients.erase(it);
        }
    }
    
    addOutputEvent(Event(closingTime, EventType::CLIENT_ARRIVED, ""));
}

void ComputerClub::addOutputEvent(const Event& event) {
    output.push_back(event.toString());
}

void ComputerClub::addError(const Time& time, const std::string& error) {
    Event errorEvent = Event::createError(time, error);
    addOutputEvent(errorEvent);
}

void ComputerClub::clientArrived(const Event& event) {
    const Time& time = event.getTime();
    const std::string& clientName = event.getClientName();
    
    if (time < openingTime || time > closingTime) {
        addError(time, "NotOpenYet");
        return;
    }
    
    if (clients.find(clientName) != clients.end()) {
        addError(time, "YouShallNotPass");
        return;
    }
    
    clients[clientName] = 0;
}

void ComputerClub::clientSat(const Event& event) {
    const Time& time = event.getTime();
    const std::string& clientName = event.getClientName();
    int tableNumber = event.getTableNumber();
    
    if (clients.find(clientName) == clients.end()) {
        addError(time, "ClientUnknown");
        return;
    }
    
    if (tableNumber < 1 || tableNumber > tablesCount) {
        addError(time, "PlaceIsBusy");
        return;
    }
    
    Table& table = tables[tableNumber - 1];
    if (!table.currentClient.empty() && table.currentClient != clientName) {
        addError(time, "PlaceIsBusy");
        return;
    }
    
    int prevTable = clients[clientName];
    if (prevTable != 0) {
        freeTable(prevTable, time);
    }
    
    sitClient(clientName, tableNumber, time);
}

void ComputerClub::clientWaiting(const Event& event) {
    const Time& time = event.getTime();
    const std::string& clientName = event.getClientName();
    
    if (clients.find(clientName) == clients.end()) {
        addError(time, "ClientUnknown");
        return;
    }
    
    for (const auto& table : tables) {
        if (table.currentClient.empty()) {
            addError(time, "ICanWaitNoLonger!");
            return;
        }
    }
    
    if (waitingQueue.size() >= static_cast<size_t>(tablesCount)) {
        Event leaveEvent(time, EventType::CLIENT_FORCE_LEFT, clientName);
        addOutputEvent(leaveEvent);
        clients.erase(clientName);
        return;
    }
    
    waitingQueue.push(clientName);
}

void ComputerClub::clientLeft(const Event& event) {
    const Time& time = event.getTime();
    const std::string& clientName = event.getClientName();
    
    if (clients.find(clientName) == clients.end()) {
        addError(time, "ClientUnknown");
        return;
    }
    
    int tableNumber = clients[clientName];
    if (tableNumber != 0) {
        freeTable(tableNumber, time);
    }
    
    clients.erase(clientName);
    
    if (!waitingQueue.empty()) {
        for (auto& table : tables) {
            if (table.currentClient.empty()) {
                std::string nextClient = waitingQueue.front();
                waitingQueue.pop();
                
                sitClient(nextClient, table.number, time);
                
                Event sitEvent(time, EventType::CLIENT_SAT_FROM_QUEUE, nextClient, table.number);
                addOutputEvent(sitEvent);
                break;
            }
        }
    }
}

void ComputerClub::freeTable(int tableNumber, const Time& currentTime) {
    Table& table = tables[tableNumber - 1];
    if (!table.currentClient.empty()) {
        Time duration = currentTime - table.startTime;
        int hours = duration.getHours();
        if (duration.getMinutes() > 0 || hours == 0) {
            hours++;
        }
        table.revenue += hours * hourCost;
        table.busyTime = table.busyTime + duration;
        table.currentClient.clear();
    }
}

void ComputerClub::sitClient(const std::string& clientName, int tableNumber, const Time& currentTime) {
    Table& table = tables[tableNumber - 1];
    table.currentClient = clientName;
    table.startTime = currentTime;
    clients[clientName] = tableNumber;
}

int ComputerClub::calculateRevenue(const Time& startTime, const Time& endTime) const {
    Time duration = endTime - startTime;
    int hours = duration.getHours();
    if (duration.getMinutes() > 0 || hours == 0) {
        hours++;
    }
    return hours * hourCost;
}

std::vector<std::string> ComputerClub::getOutput() const {
    std::vector<std::string> result = output;
    result.insert(result.begin(), openingTime.toString());
    
    for (const auto& table : tables) {
        std::ostringstream oss;
        oss << table.number << " " << table.revenue << " " << table.busyTime.toString();
        result.push_back(oss.str());
    }
    
    return result;
}
