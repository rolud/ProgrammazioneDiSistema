//
// Created by rolud on 24/06/2020.
//

#ifndef EXAMS_FUNCTIONS_H
#define EXAMS_FUNCTIONS_H

#include <iostream>
#include <mutex>
#include <string>

std::mutex mtx_print;

void tsprintln(const std::string &str) {
    std::lock_guard<std::mutex> lock(mtx_print);
    std::cout << str << std::endl;
}

#endif //EXAMS_FUNCTIONS_H
