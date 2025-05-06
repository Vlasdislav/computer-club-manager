#pragma once

#include <string>

class Time {
public:
    Time() = default;
    Time(int hours, int minutes);
    explicit Time(const std::string& timeStr);
    
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator==(const Time& other) const;
    
    int getHours() const;
    int getMinutes() const;
    std::string toString() const;
    
    int toMinutes() const;
    static Time fromMinutes(int minutes);
    
    Time operator+(const Time& other) const;
    Time operator-(const Time& other) const;
    
private:
    int hours = 0;
    int minutes = 0;
};
