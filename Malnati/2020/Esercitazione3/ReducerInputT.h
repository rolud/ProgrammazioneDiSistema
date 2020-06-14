//
// Created by rolud on 05/06/20.
//

#ifndef ESERCITAZIONE3_REDUCERINPUTT_H
#define ESERCITAZIONE3_REDUCERINPUTT_H

#include <string>

template<typename A, typename V>
class ReducerInputT {

public:
    ReducerInputT() = default;
    ReducerInputT(std::string key, V value, A accumulator) :
            m_key(std::move(key)), m_value(value), m_accumulator(accumulator) {}

    std::string getKey() const { return this->m_key; }
    V getValue() const { return this->m_value; }
    A getAccumulator() const { return this->m_accumulator; }

    std::vector<char> serialize() const {
        boost::property_tree::ptree pt;
        pt.put("key", this->m_key);
        pt.put("value", this->m_value);
        pt.put("accumulator", this->m_accumulator);
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pt);
        std::string json = oss.str();
        std::cout << "ser reducer" << json << std::endl;
        return std::vector<char>(json.begin(), json.end());
    }

    void deserialize(std::shared_ptr<char[]> json) {
        std::cout << "des reducer" ;
        boost::property_tree::ptree pt;
        std::string input { (char*) json.get() };
        std::istringstream iss(input);
        boost::property_tree::json_parser::read_json(iss, pt);
        this->m_key = pt.get<std::string>("key");
        this->m_value = pt.get<V>("value");
        this->m_accumulator = pt.get<A>("accumulator");
        std::cout << json << std::endl;
    }
private:
    std::string m_key;
    V m_value;
    A m_accumulator;
};
#endif //ESERCITAZIONE3_REDUCERINPUTT_H
