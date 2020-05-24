//
// Created by rolud on 07/08/2019.
//

#ifndef ESERCITAZIONE2_BASE_H
#define ESERCITAZIONE2_BASE_H


#include <string>

class Base {
public:
    void setName(std::string name) {this->name = name;}
    std::string getName() const {return this->name;}
    virtual int mType() const = 0;
    virtual void ls(int indent = 0) const = 0;

private:
    std::string name;
};


#endif //ESERCITAZIONE2_BASE_H
