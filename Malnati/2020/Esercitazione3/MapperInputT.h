//
// Created by rolud on 04/06/20.
//

#ifndef ESERCITAZIONE3_MAPPERINPUTT_H
#define ESERCITAZIONE3_MAPPERINPUTT_H


#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class MapperInputT {

public:
    MapperInputT() = default;
    explicit MapperInputT(std::string content) : m_content(std::move(content)) {}

    std::string getContent() const { return this->m_content; }

    std::vector<char> serialize() const {
        boost::property_tree::ptree pt;
        pt.put("content", this->m_content);
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pt);
        std::string str = oss.str();
        std::cout << "ser mapper" << str << std::endl;
        return std::vector<char>(str.begin(), str.end());
    }

    void deserialize(std::shared_ptr<char[]> json) {
        std::cout << "des mapper";
        boost::property_tree::ptree pt;
        std::string input{ (char *)(json.get()) };
        std::istringstream iss(input);
        boost::property_tree::json_parser::read_json(iss, pt);
        this->m_content = pt.get<std::string>("content");
        std::cout << json << std::endl;
    }

private:
    std::string m_content;
};


#endif //ESERCITAZIONE3_MAPPERINPUTT_H
