#include <iostream>
#include <fstream>
#include <regex>
#include "functions.h"


int main() {

    DurationLogger dl("main");

    std::regex rgx(R"(%[a-zA-Z]+%)");
    std::string path("..\\files\\regex_search");

    es4_12(path, rgx);
    es4_3(path, rgx);

//    std::string access_log_file("..\\files\\mapreduce\\access_log_simple.txt");
    std::string access_log_file("..\\files\\mapreduce\\localhost_access_log.2020.txt");
    mapreduce(access_log_file);

    test_producer_consumer();

    return 0;
}