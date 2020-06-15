//
// Created by rolud on 16/06/2020.
//

#ifndef ESERCITAZIONE4_FUNCTIONS_H
#define ESERCITAZIONE4_FUNCTIONS_H

#endif //ESERCITAZIONE4_FUNCTIONS_H

#include <thread>
#include <experimental/filesystem>


#include "DurationLogger.h"
#include "Line.h"
#include "Jobs.h"

#define N_PRODUCERS 5
#define N_CONSUMERS 4

void es4_12(const std::string &path, const std::regex &rgx) {

    DurationLogger dl("es4_12");

    Jobs<Line> jobs{};

    std::vector<std::thread> consumers{};

    std::mutex mtx_cout; // sincronizzazione dell'output

    std::thread producer { [path,&jobs](){

        std::experimental::filesystem::directory_iterator iter { path };
        std::experimental::filesystem::directory_iterator end {};

        for ( ; iter != end; iter++) {
            std::string filename(iter->path().string());
            std::ifstream ifs(filename);
            std::string line;
            int line_num = 0;
            if (ifs.is_open()) {
                while (std::getline(ifs, line)) {
                    jobs.put(Line(line, filename, ++line_num));
                }
                ifs.close();
            }
        }


    }};

    for (int i = 0; i < N_CONSUMERS; i++) {
        consumers.emplace_back( [rgx, &jobs, &mtx_cout]() {
            std::optional<Line> line;
            while ((line = jobs.get()) != std::nullopt) {
                std::string s(line.value().getValue());
                std::vector<std::smatch> res;
                auto iter = std::sregex_iterator(s.begin(), s.end(), rgx);
                for (; iter != std::sregex_iterator(); iter++) {
                    res.push_back(*iter);
                }

                if (!res.empty()) {
                    std::unique_lock<std::mutex> ul(mtx_cout);
                    std::cout << "PID " << std::this_thread::get_id() << " | ";
                    std::cout << "FILE " << line.value().getFilename() << " | ";
                    std::cout << "L " << line.value().getLine() << " | ";
                    std::cout << "MATCH " << res.size() << " | ";
                    for (auto &m: res) {
                        std::cout << m[0] << "; ";
                    }
                    std::cout << std::endl;
                }
            }

        });
    }

    producer.join();
    jobs.end();
    std::for_each(consumers.begin(), consumers.end(), [](std::thread& t){t.join();});

}

void es4_3(const std::string &path, const std::regex &rgx) {
    DurationLogger dl("es4_3");
    Jobs<std::string> file_jobs;
    Jobs<Line> line_jobs{};

    std::vector<std::thread> file_consumers; // line_producers
    std::vector<std::thread> line_consumers;

    std::mutex mtx_cout; // sincronizzazione dell'output

    //file_producer
    std::thread producer( [&path, &file_jobs](){
        std::experimental::filesystem::directory_iterator iter { path };
        std::experimental::filesystem::directory_iterator end {};

        for ( ; iter != end; iter++) {
            std::string filename(iter->path().string());
            file_jobs.put(filename);
        }
    });

    for(int i = 0; i < N_PRODUCERS; i++) {
        file_consumers.emplace_back( [&file_jobs, &line_jobs](){
            std::optional<std::string> filename;
            while((filename = file_jobs.get()) != std::nullopt) {
                std::ifstream ifs(filename.value());
                std::string line;
                int line_num = 0;
                if (ifs.is_open()) {
                    while (std::getline(ifs, line)) {
                        line_jobs.put(Line(line, filename.value(), ++line_num));
                    }
                    ifs.close();
                }
            }
        });
    }

    for (int i = 0; i < N_CONSUMERS; i++) {
        line_consumers.emplace_back( [rgx, &line_jobs, &mtx_cout]() {
            std::optional<Line> line;
            while ((line = line_jobs.get()) != std::nullopt) {
                std::string s(line.value().getValue());
                std::vector<std::smatch> res;
                auto iter = std::sregex_iterator(s.begin(), s.end(), rgx);
                for (; iter != std::sregex_iterator(); iter++) {
                    res.push_back(*iter);
                }

                if (!res.empty()) {
                    std::unique_lock<std::mutex> ul(mtx_cout);
                    std::cout << "PID " << std::this_thread::get_id() << " | ";
                    std::cout << "FILE " << line.value().getFilename() << " | ";
                    std::cout << "L " << line.value().getLine() << " | ";
                    std::cout << "MATCH " << res.size() << " | ";
                    for (auto &m: res) {
                        std::cout << m[0] << "; ";
                    }
                    std::cout << std::endl;
                }
            }

        });
    }

    producer.join();
    file_jobs.end();
    std::for_each(file_consumers.begin(), file_consumers.end(), [](std::thread& t){t.join();});
    line_jobs.end();
    std::for_each(line_consumers.begin(), line_consumers.end(), [](std::thread& t){t.join();});


}