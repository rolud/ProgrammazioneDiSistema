//
// Created by rolud on 04/06/20.
//

#ifndef ESERCITAZIONE3_MAPPERINPUTT_H
#define ESERCITAZIONE3_MAPPERINPUTT_H


#include <string>

class MapperInputT {

public:
    MapperInputT(std::string content) : m_content(std::move(content)) {}

    std::string getContent() const { return this->m_content; }
private:
    std::string m_content;
};


#endif //ESERCITAZIONE3_MAPPERINPUTT_H
