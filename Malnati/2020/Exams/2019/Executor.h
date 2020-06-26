//
// Created by rolud on 24/06/2020.
//

/**
 * È presente una classe Executor che ha il compito di rimanere in ascolto
 * su una coda di funzioni da svolgere e si appoggia su una classe esterna Context
 *
 * class Executor {
 * public:
 *      Executor(Context *ctx);
 *      ~Executor();
 *      std::future<void> submit(std::function<void(Context*)> f);
 *      void shutdown();
 * };
 *
 * Quando viene costruita la classe, viene generato un unico thread che ha il
 * compito di estrarre le funzioni dalla coda, immesse tramite il metodo submit(),
 * ed eseguirle.
 * Esso termina quando viene chiamato il metodo shutdown() o il distruttore della classe.
 * Le funzioni da svolgere vengono immesse tramite un metodo submit, che torna un oggetto
 * future col risultato dell'elaborazione.
 * Se il metodo viene chiamato quando è già stato chiamato il metodo shutdown, viene
 * lanciata un'eccezione di tipo std::logic_error.
 * Il metodo shutdown viene chiamato per terminare l'esecuzione, e se ci sono ancora
 * delle funzioni in coda, esse vengono comunque svolte.
 */
#ifndef EXAMS_EXECUTOR_H
#define EXAMS_EXECUTOR_H

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <functional>
#include "Context.h"

class Executor {
public:
    explicit Executor(Context *ctx) : m_ctx(ctx), m_finish(false){
        m_thread = std::thread {[this](){
            while(true) {
                std::unique_lock<std::mutex> lock(this->m_mutex);
                this->m_cv.wait(lock, [this](){ return !this->m_queue.empty() || this->m_finish; });
                if (this->m_finish && this->m_queue.empty()) break;
                auto pt = std::move(this->m_queue.front());
                this->m_queue.pop();
                if (pt.get_future().wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
                    pt.get_future().wait();
            }
        }};
    }

    ~Executor() {
        if (!this->m_finish)
            shutdown();
        this->m_thread.join();
    };

    std::future<void> submit(std::function<void(Context*)> f) {
        if (this->m_finish)
            throw std::logic_error("Submit called after shutdown.");
        std::unique_lock<std::mutex> lock(this->m_mutex);
        std::packaged_task<void(Context*)> pt(std::bind(f, this->m_ctx));
        std::future<void> ftr = pt.get_future();
        this->m_queue.push(std::move(pt));
        this->m_cv.notify_one();
        return ftr;
    }

    void shutdown() {
        this->m_finish = true;
        this->m_cv.notify_all();
    }

private:
    Context *m_ctx;
    std::atomic<bool> m_finish;
    std::thread m_thread;
    std::queue<std::packaged_task<void(Context*)>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

#endif //EXAMS_EXECUTOR_H
