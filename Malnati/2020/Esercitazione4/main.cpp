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

//    std::string access_log("..\\files\\mapreduce\\access_log_simple.txt");
    std::string access_log("..\\files\\mapreduce\\localhost_access_log.2020.txt");
    std::string mapreduce_log_file(access_log);

    mapreduce(mapreduce_log_file);

    return 0;
}