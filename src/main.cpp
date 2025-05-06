#include <iostream>
#include <fstream>
#include <vector>
#include "ComputerClub.hpp"
#include "Event.hpp"

std::vector<std::string> readLines(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    try {
        auto lines = readLines(argv[1]);
        if (lines.size() < 3) {
            throw std::runtime_error("Invalid input format");
        }
        
        int tablesCount = std::stoi(lines[0]);
        
        std::istringstream timeIss(lines[1]);
        std::string openTimeStr, closeTimeStr;
        timeIss >> openTimeStr >> closeTimeStr;
        Time openingTime(openTimeStr);
        Time closingTime(closeTimeStr);
        
        int hourCost = std::stoi(lines[2]);
        
        ComputerClub club(tablesCount, openingTime, closingTime, hourCost);
        
        for (size_t i = 3; i < lines.size(); ++i) {
            if (!lines[i].empty()) {
                Event event = Event::parse(lines[i]);
                club.processEvent(event);
            }
        }
        
        club.endOfDay();
        
        auto output = club.getOutput();
        for (const auto& line : output) {
            std::cout << line << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
