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

    for(std::thread &t : this->threadpool)
        if(t.joinable()) {
            t.join();
        }

    int endtime = std::clock();
    std::cout << "STATS" << std::endl;
    // todo
}

void JobScheduler::submit(Job j) {
    this->waiting_queue.push(j);
}

void JobScheduler::start() {
    this->starttime = std::clock();

    // threads definition
    for (int i = 0; i < this->poolsize; i++) {
        this->threadpool.emplace_back(std::thread([&](){
            while (true) {
                std::unique_lock<std::mutex> lk(this->mutex);
                this->cv_runnings.wait(lk, [&](){return !this->running_queue.empty();});
                Job j = this->running_queue.front();
                this->running_queue.pop();
                std::cout << "thread " << std::this_thread::get_id() << " running job " << j.getId() << " at t = " << (std::clock() - this->starttime) << std::endl;
                lk.unlock();
                int tsleep = (j.getDuration() < JobScheduler::EXTIME) ? j.getDuration() : JobScheduler::EXTIME;
                std::this_thread::sleep_for(std::chrono::milliseconds(tsleep));
                if (tsleep + j.getExecution_time() >= j.getDuration()) {
                    // job terminated
                    j.setExecution_time(tsleep);
                    this->terminated_jobs.push_back(j);
                    //std::cout << std::this_thread::get_id() << " finishes job " << j.getId() << std::endl;
                    std::cout << "job " << j.getId() << " done at t = " << (std::clock() - this->starttime) << std::endl;
                } else {
                    // job not terminted -> EXTIME
                    j.setExecution_time(tsleep);
                    this->running_queue.push(j);
                    //std::cout << std::this_thread::get_id() << " stops job " << j.getId() << std::endl;
                    this->cv_runnings.notify_one();

                }

                if (this->running_queue.empty() && this->waiting_queue.empty())
                    break;
            }

        }));
    }

    while (!this->waiting_queue.empty()) {
        Job j = this->waiting_queue.top();
        auto elapsed = std::clock() - this->starttime;
        if ( elapsed >= j.getStart_time()) {
            this->waiting_queue.pop();
            this->running_queue.push(j);
            this->cv_runnings.notify_one();
//            std::cout << "job -> " << j.getId() << " elapsed: " << elapsed << std::endl;
        }
    }


    //while (true) ;


}