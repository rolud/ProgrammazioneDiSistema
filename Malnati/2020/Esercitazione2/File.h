//
// Created by rolud on 02/06/2020.
//

#ifndef ESERCITAZIONE2_FILE_H
#define ESERCITAZIONE2_FILE_H


#include "Base.h"

class File : public Base {

public:
    File(std::string name, uintmax_t size);
    Base::Type mType() const override;
    void ls(int indent) const override;

private:
    uintmax_t m_size;
};


#endif //ESERCITAZIONE2_FILE_H
