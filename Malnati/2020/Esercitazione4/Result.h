//
// Created by rolud on 16/06/2020.
//

#ifndef ESERCITAZIONE4_RESULT_H
#define ESERCITAZIONE4_RESULT_H

#include <string>

template <class T>
class Result {
public:
    Result()= default;
    explicit Result(std::string key): m_key(std::move(key)) {}
    Result(std::string key, T value): m_key(std::move(key)), m_value(value) {}

    std::string getKey() const { return  this->m_key; }
    T getValue() const { return this->m_value; }

private:
    std::string m_key;
    T m_value;
};

#endif //ESERCITAZIONE4_RESULT_H
