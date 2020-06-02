//
// Created by rolud on 02/06/2020.
//

#ifndef ESERCITAZIONE2_BASE_H
#define ESERCITAZIONE2_BASE_H

#include <string>

class Base {
public:
    enum Type {FILE, DIRECTORY};

    std::string getName() const { return this->m_name; }
    virtual Type mType() const = 0;
    virtual void ls(int indent) const = 0;

protected:
    std::string m_name;

};

#endif //ESERCITAZIONE2_BASE_H
