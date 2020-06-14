#include <iostream>
#include <fstream>

#include <regex>
#include <thread>
#include "DurationLogger.h"
#include "Jobs.h"

void es4_1(const std::string& path, const std::regex& rgx) {

    DurationLogger dl("es4_1");

    Jobs<std::string> jobs{};

    std::thread producer { [path,&jobs](){
        std::ifstream ifs(path);
        std::string line;
        if (ifs.is_open()) {
            while (std::getline(ifs, line)) {
                jobs.put(line);
            }
            ifs.close();
        }
    }};

    std::thread consumer { [rgx,&jobs](){
        std::optional<std::string> line;
        while ((line = jobs.get()) != std::nullopt) {
            std::smatch match;
            std::vector<std::smatch> res;
            auto iter = std::sregex_iterator(line.value().begin(), line.value().end(), rgx);
            for ( ; iter != std::sregex_iterator(); iter++) {
               res.push_back(*iter);
            }

            if (!res.empty()) {
                std::cout << "Match number: " << res.size() << " -> ";
                for (auto &m: res) {
                    std::cout << m[0] << " | ";
                }
                std::cout << std::endl;
            }
        }

    } };

    producer.join();
    jobs.end();
    consumer.join();
}

int main() {

    std::regex rgx(R"(%[a-z]+%)");
    std::string path("../file1.txt");

    es4_1(path, rgx);

    return 0;
}