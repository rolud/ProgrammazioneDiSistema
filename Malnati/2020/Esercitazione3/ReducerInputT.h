//
// Created by rolud on 05/06/20.
//

#ifndef ESERCITAZIONE3_REDUCERINPUTT_H
#define ESERCITAZIONE3_REDUCERINPUTT_H

#include <string>

template<typename A, typename V>
class ReducerInputT {

public:
    ReducerInputT(std::string key, V value, A accumulator) :
            m_key(std::move(key)), m_value(value), m_accumulator(accumulator) {}

    std::string getKey() const { return this->m_key; }
    V getValue() const { return this->m_value; }
    A getAccumulator() const { return this->m_accumulator; }
private:
    std::string m_key;
    V m_value;
    A m_accumulator;
};
#endif //ESERCITAZIONE3_REDUCERINPUTT_H
