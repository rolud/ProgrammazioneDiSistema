//
// Created by rolud on 04/06/20.
//

#ifndef ESERCITAZIONE3_RESULTT_H
#define ESERCITAZIONE3_RESULTT_H

#include <string>

template<typename T>
class ResultT {

public:
    ResultT() = default;
    ResultT(std::string key) : m_key(std::move(key)) {}
    ResultT(std::string key, T value) : m_key(std::move(key)), m_value(value) {}

    std::string getKey() const { return this->m_key; }
    T getValue() const { return this->m_value; }
private:
    std::string m_key;
    T m_value;
};


#endif //ESERCITAZIONE3_RESULTT_H
