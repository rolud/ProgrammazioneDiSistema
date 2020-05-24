//
// Created by rolud on 24/05/2020.
//

#include <algorithm>
#include <cstring>
#include "MyString.h"

MyString::MyString(int size) : size(size) {
    str = new char[size];
}

MyString::MyString(const MyString &that) {
    delete[] str;
    this->str = nullptr;
    size = that.size;
    str = new char[size];
    std::memcpy(this->str, that.str, (size_t) size);
}

MyString::MyString(MyString &&that) : size(0), str(nullptr){
    swap(*this, that);
}

MyString::~MyString() {
    delete[] str;
}

MyString& MyString::operator=(MyString source) {
    swap(*this, source);
    return *this;
}


int MyString::getSize() const { return size; }

char* MyString::getString() const { return str; }

void swap(MyString &a, MyString &b) {
    std::swap(a.size, b.size);
    std::swap(a.str, b.str);
}

