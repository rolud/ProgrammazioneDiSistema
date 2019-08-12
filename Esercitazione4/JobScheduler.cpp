//
// Created by rolud on 12/08/2019.
//

#include <iostream>
#include "JobScheduler.h"

JobScheduler::JobScheduler() {
    this->poolsize = std::thread::hardware_concurrency();
    if (this->poolsize < 2) this->poolsize = 2;
    this->threadpool.reserve(this->poolsize);
}

JobScheduler::~JobScheduler() {
    std::cout << "STATS" << std::endl;
    // todo
}

void JobScheduler::submit(Job j) {
    this->waiting_queue.push(j);
}

void JobScheduler::start() {
    this->starttime = std::chrono::system_clock::now();
    // todo
}