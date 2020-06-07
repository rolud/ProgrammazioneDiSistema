//
// Created by rolud on 04/06/20.
//

#ifndef ESERCITAZIONE3_MAPPERINPUTT_H
#define ESERCITAZIONE3_MAPPERINPUTT_H


#include <string>

class MapperInputT {

public:
    explicit MapperInputT(std::string content) : m_content(std::move(content)) {}

    std::string getContent() const { return this->m_content; }

    std::vector<char> serialize() const {

    }

    void deserialize(std::shared_ptr<char*> json) {

    }

private:
    std::string m_content;
};


#endif //ESERCITAZIONE3_MAPPERINPUTT_H
