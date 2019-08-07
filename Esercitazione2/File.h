//
// Created by rolud on 07/08/2019.
//

#ifndef ESERCITAZIONE2_FILE_H
#define ESERCITAZIONE2_FILE_H


#include "Base.h"

class File : public Base {
public:
    File(std::string name, uintmax_t size);
    uintmax_t getSize() const;
    void ls(int indent = 0) const override;
    int mType() const override;
private:
    uintmax_t size;
};


#endif //ESERCITAZIONE2_FILE_H
