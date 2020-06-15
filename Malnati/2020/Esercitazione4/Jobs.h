//
// Created by rolud on 14/06/2020.
//

#ifndef UNTITLED_JOBS_H
#define UNTITLED_JOBS_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <shared_mutex>

#define MAX_QUEUE_SIZE 5

template <class T>
class Jobs {


public:

    Jobs() : m_end(false) {}

    /**
     * inserisce un job in coda in attesa di essere processato,
     * può essere bloccante se la coda dei job è piena
     */
    void put(T job) {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_cv_put.wait(lock, [&](){ return this->m_jobs.size() < MAX_QUEUE_SIZE; });
        m_jobs.push(job);
        this->m_cv_get.notify_one();
    }

    /**
     * legge un job dalla coda e lo rimuove
     * si blocca se non ci sono valori in coda
     */
    std::optional<T> get() {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_cv_get.wait(lock, [&](){ return this->m_jobs.size() > 0 || (this->m_jobs.size() == 0 && this->m_end); });
        if (m_jobs.size() == 0 && this->m_end) return {};
        T t = m_jobs.front();
        m_jobs.pop();
        this->m_cv_put.notify_one();
        return t;
    }

    void end() {
        this->m_end = true;
    }


private:
    std::queue<T> m_jobs;
    std::mutex m_mtx;
    std::condition_variable m_cv_get, m_cv_put;
    bool m_end;
};
#endif //UNTITLED_JOBS_H
