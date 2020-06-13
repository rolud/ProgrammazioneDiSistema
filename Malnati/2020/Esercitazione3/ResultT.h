//
// Created by rolud on 04/06/20.
//

#ifndef ESERCITAZIONE3_RESULTT_H
#define ESERCITAZIONE3_RESULTT_H

#include <string>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

template<typename T>
class ResultT {

public:
    ResultT() = default;
    ResultT(std::string key) : m_key(std::move(key)) {}
    ResultT(std::string key, T value) : m_key(std::move(key)), m_value(value) {}

    std::string getKey() const { return this->m_key; }
    T getValue() const { return this->m_value; }

    std::vector<char> serialize() {
        boost::property_tree::ptree pt;
        pt.put("key", this->m_key);
        pt.put("value", this->m_value);
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pt);
        std::string json = oss.str();
        return std::vector<char>(json.begin(), json.end());
    }

    void deserialize(std::shared_ptr<char*> json) {
        boost::property_tree::ptree pt;
        std::string input { (char*) json.get() };
        std::istringstream iss(input);
        boost::property_tree::json_parser::read_json(iss, pt);
        this->m_key = pt.get<std::string>("key");
        this->m_value = pt.get<T>("value");
    }
private:
    std::string m_key;
    T m_value;
};


#endif //ESERCITAZIONE3_RESULTT_H
