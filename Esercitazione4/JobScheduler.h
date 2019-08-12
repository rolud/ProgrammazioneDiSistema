//
// Created by rolud on 12/08/2019.
//

#ifndef ESERCITAZIONE4_JOBSCHEDULER_H
#define ESERCITAZIONE4_JOBSCHEDULER_H


#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Job.h"

class JobScheduler {
public:
    JobScheduler();
    ~JobScheduler();
    void submit(Job j);
    void start();

private:
    const int EXTIME = 3000;
    int poolsize;
    std::chrono::system_clock::time_point starttime;

    std::vector<std::thread> threadpool;
    std::priority_queue<Job> waiting_queue;
    std::queue<Job> running_queue;
    std::vector<Job> terminated_jobs;

    std::mutex mutex;
    std::condition_variable cv_waintings;
    std::condition_variable cv_runnings;
};


#endif //ESERCITAZIONE4_JOBSCHEDULER_H
