//
// Created by rolud on 24/06/2020.
//

#ifndef EXAMS_TEST_FUNCTIONS_H
#define EXAMS_TEST_FUNCTIONS_H

#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "DurationLogger.h"
#include "2016/Exchanger.h"
#include "2019/Context.h"
#include "2019/Executor.h"
#include "nodate/GameStanding.h"

#include "functions.h"
#include "nodate/Phaser.h"

void exchanger() {
    DurationLogger dl("exchanger");
    Exchanger<std::string> exchanger;

    auto f = [&exchanger](){
        std::stringstream s;
        s << "Thread " << std::this_thread::get_id();
        tsprintln(s.str());
        s.str("");
        s << "obj of " << std::this_thread::get_id();
        std::string res = exchanger.exchange(s.str());
        s.str("");
        s << "Thread " << std::this_thread::get_id() << " -> " << res;
        tsprintln(s.str());
    };

    std::thread thread1(f);
    std::thread thread2(f);

    thread1.join();
    thread2.join();
}

void exchanger_brute_force_test() {
    DurationLogger dl("exchanger brute force test");
    Exchanger<std::thread::id> exchanger;
    std::map<std::thread::id, std::thread::id> results;
    std::vector<std::thread> thread_pool;

    auto f = [&exchanger,&results]() {
        std::stringstream s;
        s << "Thread " << std::this_thread::get_id();
        tsprintln(s.str());
        std::thread::id res = exchanger.exchange(std::this_thread::get_id());
        results[std::this_thread::get_id()] = res;
    };

    for (int i = 0; i < 1024; ++i) {
        std::thread t1(f);
        std::thread t2(f);
        t1.join();
        t2.join();
    }

    bool passed = true;
    auto iter = results.begin();
    for ( ; iter != results.end(); ++iter)
        if (results[iter->second] != iter->first) {
            passed = false;
            break;
        }

    if (passed)
        std::cout << "EXCHANGER BRUTE FORCE TEST : PASSED" << std::endl;
    else
        std::cout << "EXCHANGER BRUTE FORCE TEST : FAILED" << std::endl;

}

void gameStanding() {
    DurationLogger dl("game standing");
    GameStanding gs;

    std::thread t1 { [&gs](){
        gs.push("rolud", 16);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("pippo", 19);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("ax", 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("rolud", 23);
    }};

    std::thread t2 { [&gs](){
        gs.push("venere", 16);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("matteo", 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("elviro", 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        gs.push("pippo", 21);
        gs.push("matteo", 3);
    }};

    /*
     * Expected at the end:
     * 1. rolud 23
     * 2. pippo 21
     * 3. venere 16
     * 4. matteo 10
     * 5. elviro 10
     * 6. ax 1
     */

    std::thread t3 { [&gs](){
        int count_get_standings = 0;
        gs.listenForStandingUpdate([&](){
            auto res = gs.getStanding();
            tsprintln("Standing");
            int i = 1;
            for (auto &r : res) {
                std::stringstream s;
                s << i << ". " << r.first << ": " << r.second;
                tsprintln(s.str());
                i++;
            }
        });
    } };

    t1.join();
    t2.join();
    gs.finish();
    t3.join();

}

void phaser() {
    DurationLogger dl("phaser");

    Phaser phaser(4);

    std::vector<std::thread> threads;

    auto f = [&phaser]() {
        std::stringstream s;
        s << "Thread " << std::this_thread::get_id() << " | attending...";
        tsprintln(s.str());
        phaser.attendi();
        s.str("");
        s << "Thread " << std::this_thread::get_id() << " | waked up";
        tsprintln(s.str());
    };

    for (int i = 0; i < 4; ++i) threads.emplace_back(f);

    for (auto& t : threads) t.join();

}

void executor() {
    DurationLogger dl("executor");

    auto *ctx = new Context;
    Executor executor(ctx);

    executor.submit([](Context *ctx){
        std::cout << "EXE1" << std::endl;
    });
    executor.submit([](Context *ctx){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "EXE2" << std::endl;
    });
    executor.submit([](Context *ctx){
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        std::cout << "EXE3" << std::endl;
    });
    executor.submit([](Context *ctx){
        std::cout << "EXE4" << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    executor.shutdown();
    delete ctx;
}

#endif //EXAMS_TEST_FUNCTIONS_H
