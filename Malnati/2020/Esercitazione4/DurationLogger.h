//
// Created by rolud on 24/05/2020.
//

#ifndef ESERCITAZIONE1_DURATIONLOGGER_H
#define ESERCITAZIONE1_DURATIONLOGGER_H


#include <ctime>
#include <string>
#include <iostream>

class DurationLogger {
    std::string name;
    clock_t startTimeClocks;
    time_t startTimeSeconds;

public:
    DurationLogger(const std::string &name) : name(name) {
        startTimeClocks = std::clock();
        startTimeSeconds = std::time(nullptr);
        std::cout << " >>>>>>>> starting " << name << std::endl;
    }
    ~DurationLogger() {
        std::cout << " >>>>>>>>   ending " << name
            << " : " << std::time(nullptr) - startTimeSeconds << " sec "
            << "(" << std::clock() - startTimeClocks << " clocks)" << std::endl;

    }
};


#endif //ESERCITAZIONE1_DURATIONLOGGER_H
