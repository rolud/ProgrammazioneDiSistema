//
// Created by rolud on 24/06/2020.
//

/**
 * La classe Phaser ha i metodi "void attendi()", "void aggiungi()" e "void rimuovi()".
 * La classe permette a N-1 thread di attendersi tra loro, ma quando si arriva
 * a N-1 i thread si sbloccano. I metodi aggiungi e rimuovi modificano il valore di N
 * (specificato nella costruzione dell'oggetto) e di conseguenza sbloccano i thread se
 * necessario.
 */

#ifndef EXAMS_PHASER_H
#define EXAMS_PHASER_H

#include <atomic>
#include <condition_variable>
#include <mutex>

class Phaser {
public:

    explicit Phaser(int n) : m_n_max(n), m_n(0), m_unlock(false) {}

    // quando uno si sblocca,
    // diminuisce il contatore e
    // sveglia un altro thread,
    // gli altri thread vengono svegliati a catena
    void attendi() {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_n++;
        if (this->m_n == this->m_n_max)
            this->m_unlock = true;
        else
            this->m_cv.wait(lock, [this](){ return this->m_n == this->m_n_max || this->m_unlock; });
        if (this->m_n == 0)
            this->m_unlock = false;
        else
            this->m_n--;
        this->m_cv.notify_one();
    }

    void aggiungi(int n) {
        m_n_max.fetch_add(n);
        this->m_cv.notify_one();
    }

    void rimuovi(int n) {
        m_n_max.fetch_sub(n);
        this->m_cv.notify_one();
    }

private:
    int m_n;
    std::atomic<int> m_n_max;
    bool m_unlock;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

#endif //EXAMS_PHASER_H
