//
// Created by rolud on 24/05/2020.
//

#include <iostream>
#include "Buffer.h"


Buffer::Buffer(int size) : size(size) {
    buffer = new int[size];
    std::cout << " -- constructor -- " << std::endl;
}

Buffer::~Buffer() {
    delete[] buffer;
    std::cout << " -- destructor -- " << std::endl;
}

int Buffer::getSize() {
    return size;
}

bool Buffer::getValue(int pos, int  &val) {
    if (pos < 0 || pos >= size) return false;
    val = buffer[pos];
    return true;
}

bool Buffer::setValue(int pos, int val) {
    if (pos < 0 || pos >= size) return false;
    buffer[pos] = val;
    return true;
}