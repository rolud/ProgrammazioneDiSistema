#include <iostream>
#include <fstream>
#include <regex>
#include "functions.h"


int main() {

    std::regex rgx(R"(%[a-zA-Z]+%)");
    std::string path("..\\filesdir");

    es4_12(path, rgx);
    es4_3(path, rgx);

    return 0;
}