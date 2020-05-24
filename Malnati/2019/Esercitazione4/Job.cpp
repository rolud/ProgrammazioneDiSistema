//
// Created by rolud on 12/08/2019.
//

#include "Job.h"

Job::Job(int id, int start_time, int duration) {
    this->id = id;
    this->start_time = start_time;
    this->duration = duration;
    this->execution_time = this->wait_time = this->completation_time = 0;
}

int Job::getId() const {
    return id;
}

void Job::setId(int id) {
    Job::id = id;
}

int Job::getDuration() const {
    return duration;
}

void Job::setDuration(int duration) {
    Job::duration = duration;
}

int Job::getExecution_time() const {
    return execution_time;
}

void Job::setExecution_time(int execution_time) {
    Job::execution_time += execution_time;
}

int Job::getStart_time() const {
    return start_time;
}

void Job::setStart_time(int start_time) {
    Job::start_time + start_time;
}

int Job::getWait_time() const {
    return wait_time;
}

void Job::setWait_time(int wait_time) {
    Job::wait_time += wait_time;
}

int Job::getCompletation_time() const {
    return completation_time;
}

void Job::setCompletation_time(int completation_time) {
    Job::completation_time = completation_time;
}

int Job::getLast_wait() const {
    return last_wait;
}

void Job::setLast_wait(int last_wait) {
    Job::last_wait = last_wait;
}

// if start time is lower then priority is higher
bool Job::operator<(const Job &j) const {
    return this->start_time > j.start_time;
}

bool Job::operator>(const Job &j) const {
    return this->start_time < j.start_time;
}
