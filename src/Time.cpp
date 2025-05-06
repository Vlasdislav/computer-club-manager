#include "Time.hpp"
#include <stdexcept>
#include <iomanip>

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        throw std::invalid_argument("Invalid time");
    }
}

Time::Time(const std::string& timeStr) {
    if (timeStr.size() != 5 || timeStr[2] != ':') {
        throw std::invalid_argument("Invalid time format");
    }
    
    hours = std::stoi(timeStr.substr(0, 2));
    minutes = std::stoi(timeStr.substr(3, 2));
    
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        throw std::invalid_argument("Invalid time");
    }
}

bool Time::operator<(const Time& other) const {
    return hours < other.hours || (hours == other.hours && minutes < other.minutes);
}

bool Time::operator<=(const Time& other) const {
    return *this < other || *this == other;
}

bool Time::operator>(const Time& other) const {
    return !(*this <= other);
}

bool Time::operator>=(const Time& other) const {
    return !(*this < other);
}

bool Time::operator==(const Time& other) const {
    return hours == other.hours && minutes == other.minutes;
}

int Time::getHours() const { return hours; }
int Time::getMinutes() const { return minutes; }

std::string Time::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

int Time::toMinutes() const {
    return hours * 60 + minutes;
}

Time Time::fromMinutes(int minutes) {
    return Time(minutes / 60, minutes % 60);
}

Time Time::operator+(const Time& other) const {
    int totalMinutes = toMinutes() + other.toMinutes();
    return fromMinutes(totalMinutes);
}

Time Time::operator-(const Time& other) const {
    int totalMinutes = toMinutes() - other.toMinutes();
    return fromMinutes(totalMinutes);
}
