//
// Created by rolud on 07/08/2019.
//

#include <iostream>
#include "File.h"

File::File(std::string name, uintmax_t size) {
    this->setName(name);
    this->size = size;
}

uintmax_t File::getSize() const {
    return this->size;
}

void File::ls(int indent) const {
    for (int i = 0; i < indent; i++)
        std::cout << "\t";
    std::cout << this->getName() << std::endl;
}

int File::mType() const {
    return 0;
}

