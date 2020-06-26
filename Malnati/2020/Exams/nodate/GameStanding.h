//
// Created by rolud on 24/06/2020.
//

/**
 * Una classe concorrente gestisce la classifica di un gioco online: ogni
 * concorrente è identificato da un nickname e ha un punteggio associato. Un metodo
 * ritorna una copia della classifica, un altro inserisce un nuovo punteggio relativo a un
 * concorrente (inserendo un nuovo concorrente in classifica se non esistente o
 * aggiornando il punteggio se maggiore di quello vecchio), un metodo si mette in attesa
 * di variazioni sulla classifica.
 */
#ifndef EXAMS_CLASSIFICA_H
#define EXAMS_CLASSIFICA_H

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <functional>

class GameStanding {
public:

    explicit GameStanding() : m_finish(false) {}

    void push(const std::string& nickname, long points) {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        auto old = this->m_standing.find(nickname);
        if (!(old != this->m_standing.end() && old->second >= points )) {
            this->m_standing[nickname] = points;
            this->m_cv.notify_one();
        }
    }

    std::vector<std::pair<std::string, long>> getStanding() const {
        std::multimap<long, std::pair<std::string, long>, std::greater<>> tmp;
        auto iter = this->m_standing.begin();
        for ( ; iter != m_standing.end(); ++iter) {
            tmp.insert(std::make_pair(iter->second, std::make_pair(iter->first, iter->second)));
        }

        std::vector<std::pair<std::string, long>> res;
        auto tmp_iter = tmp.begin();
        for ( ; tmp_iter != tmp.end(); ++tmp_iter) {
            res.emplace_back(tmp_iter->second);
        }
        return res;
    }

    void listenForStandingUpdate(std::function<void (void)> f) {
        while (!m_finish) {
            std::unique_lock<std::mutex> lock(this->m_mtx);
            this->m_cv.wait(lock);
            f();
        }
    }

    void finish() {
        m_finish = true;
        m_cv.notify_all();
    }

private:
    /** mappa non ordinata che rapprensenta la classifica */
    std::map<std::string, long> m_standing;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::atomic<bool> m_finish;
};

#endif //EXAMS_CLASSIFICA_H
