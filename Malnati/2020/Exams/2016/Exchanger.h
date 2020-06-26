//
// Created by rolud on 24/06/2020.
//

/**
 * La classe generica Exchanger<T> permette a due thread di scambiarsi un valore di
 * tipo T. Essa offre il metodo T exchange(T t) che blocca il thread corrente fino a che
 * un altro thread non invoca lo stesso metodo sulla stessa istanza. Quando questo
 * avviene, il metodo restituisce l'oggetto passato come parametro dal thread opposto.
 * Si implementi la classe C++, usando la libreria standard C++11.
 */

#ifndef EXAMS_EXCHANGER_H
#define EXAMS_EXCHANGER_H

#include <condition_variable>
#include <mutex>
#include <optional>

template <class T>
class Exchanger {
public:

    explicit Exchanger() : m_n_thread(0) {
        this->m_obj_first_thread = std::nullopt;
        this->m_obj_second_thread = std::nullopt;
    }

    T exchange(T t) {
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_n_thread++;
        T res;
        if (this->m_n_thread == 1) {
            this->m_obj_first_thread = std::nullopt;
            this->m_obj_second_thread = std::nullopt;
            this->m_obj_first_thread = t;
            this->m_cv.wait(lock, [this]() { return m_obj_second_thread != std::nullopt; });
            res = this->m_obj_second_thread.value();
        } else if (this->m_n_thread == 2) {
            this->m_obj_second_thread = t;
            res = this->m_obj_first_thread.value();
            this->m_cv.notify_one();
        }
        this->m_n_thread--;
        return res;
    }
private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    int m_n_thread;
    std::optional<T> m_obj_first_thread;
    std::optional<T> m_obj_second_thread;
};

#endif //EXAMS_EXCHANGER_H
