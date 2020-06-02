//
// Created by rolud on 02/06/2020.
//

#include <iostream>
#include "File.h"

File::File(std::string name, uintmax_t size) : m_size(size) {
    Base::m_name = name;
}

Base::Type File::mType() const { return Base::FILE; }

void File::ls(int indent) const {
    std::cout << std::string(indent * 4, ' ') << Base::m_name << " " << this->m_size << std::endl;
}
