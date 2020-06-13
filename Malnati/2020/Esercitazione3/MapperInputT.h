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
    explicit MapperInputT(std::string content) : m_content(std::move(content)) {}

    std::string getContent() const { return this->m_content; }

    std::vector<char> serialize() const {
        boost::property_tree::ptree pt;
        pt.put("content", this->m_content);
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pt);
        std::string str = oss.str();
        return std::vector<char>(str.begin(), str.end());
    }

    void deserialize(std::shared_ptr<char*> json) {
        boost::property_tree::ptree pt;
        std::string input{ *(json.get()) };
        std::istringstream iss(input);
        boost::property_tree::json_parser::read_json(iss, pt);
        this->m_content = pt.get<std::string>("content");
        std::cout << "deserialize ->" << this->m_content << std::endl;
    }

private:
    std::string m_content;
};


#endif //ESERCITAZIONE3_MAPPERINPUTT_H
