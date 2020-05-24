//
// Created by rolud on 12/08/2019.
//

#include <iostream>
#include "JobScheduler.h"

JobScheduler::JobScheduler() {
    this->poolsize = 4;//std::thread::hardware_concurrency();
    if (this->poolsize < 2) this->poolsize = 2;
    this->threadpool.reserve(this->poolsize);
}

JobScheduler::~JobScheduler() {

    for(std::thread &t : this->threadpool)
        if(t.joinable()) {
            t.join();
        }

    int endtime = std::clock();
    std::cout << "===== STATS =====" << std::endl;
    for (Job j : this->terminated_jobs) {
        std::cout << "--> job " << j.getId() << std::endl;
        std::cout << "    start time ......... " << j.getStart_time() << std::endl;
        std::cout << "    completation time: . " << j.getCompletation_time() << std::endl;
        std::cout << "    execution time: .... " << j.getExecution_time() << std::endl;
        std::cout << "    wait time: ......... " << j.getWait_time() << std::endl;
    }
    // todo
}

void JobScheduler::submit(Job j) {
    this->waiting_queue.push(j);
}

void JobScheduler::start() {
    auto njobs = this->waiting_queue.size();
    this->starttime = std::clock();
    std::cout << "+++ reamining " << this->waiting_queue.size() << " waiting and " << this->running_queue.size() << " running" << std::endl;

    // threads definition
    for (int i = 0; i < this->poolsize; i++) {
        this->threadpool.emplace_back(std::thread([&](){
            int start = 0;
            while (true) {
                std::unique_lock<std::mutex> lk(this->mutex);
                this->cv_runnings.wait(lk);
                if (this->running_queue.empty() && this->waiting_queue.empty()) {
                    cv_runnings.notify_all();
                    std::cout << "--- thread " << std::this_thread::get_id() << " stops " << std::endl;
                    std::cout << "=== reamining " << this->waiting_queue.size() << " waiting and " << this->running_queue.size() << " running" << std::endl;
                    break;
                }
                if (this->running_queue.empty()) continue;
                Job j = this->running_queue.front();
                this->running_queue.pop();
                if (j.getStart_time() == 0) j.setStart_time(std::clock() - this->starttime);
                j.setWait_time(std::clock() - j.getLast_wait());
                std::cout << "thread " << std::this_thread::get_id() << " running job " << j.getId() << " at t = " << (std::clock() - this->starttime) << std::endl;
                lk.unlock();
                int tsleep = (j.getDuration() < JobScheduler::EXTIME) ? j.getDuration() : JobScheduler::EXTIME;
                std::this_thread::sleep_for(std::chrono::milliseconds(tsleep));
                if (tsleep + j.getExecution_time() >= j.getDuration()) {
                    // job terminated
                    j.setExecution_time(tsleep);
                    j.setCompletation_time(std::clock() - this->starttime);
                    this->terminated_jobs.push_back(j);
                    //std::cout << std::this_thread::get_id() << " finishes job " << j.getId() << std::endl;
                    std::cout << "job " << j.getId() << " done at t = " << (std::clock() - this->starttime) << std::endl;
                } else {
                    // job not terminted -> EXTIME
                    j.setExecution_time(tsleep);
                    j.setLast_wait(std::clock());
                    this->running_queue.push(j);
                    //std::cout << std::this_thread::get_id() << " stops job " << j.getId() << std::endl;
                    std::cout << "+++ reamining " << this->waiting_queue.size() << " waiting and " << this->running_queue.size() << " running" << std::endl;
                    this->cv_runnings.notify_one();

                }

                if (this->running_queue.empty() && this->waiting_queue.empty()) {
                    cv_runnings.notify_all();
                    std::cout << "--- thread " << std::this_thread::get_id() << " stops " << std::endl;
                    std::cout << "=== reamining " << this->waiting_queue.size() << " waiting and " << this->running_queue.size() << " running" << std::endl;
                    break;
                }
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