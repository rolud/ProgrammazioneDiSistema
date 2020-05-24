//
// Created by rolud on 07/08/2019.
//

#include <cstdlib>
#include "StringBuffer.h"

StringBuffer::StringBuffer() {
    StringBuffer::str = new char[StringBuffer::BUFFERSIZE];
    memcpy(StringBuffer::str, "\0", sizeof(char));
    StringBuffer::buffer_size = BUFFERSIZE;
    StringBuffer::str_length = 0;
}

StringBuffer::StringBuffer(const char *str) {
    if (str != nullptr) {
        StringBuffer::str = new char[strlen(str)];
        memcpy(StringBuffer::str, str, strlen(str));
        memcpy(StringBuffer::str + strlen(str), "\0", sizeof(char));
        StringBuffer::buffer_size = strlen(str) + 2;
        StringBuffer::str_length = strlen(str);
    }
}

StringBuffer::StringBuffer(const StringBuffer &sb) {
    StringBuffer::str = new char[sb.buffer_size];
    memcpy(StringBuffer::str, sb.str, sb.str_length);
    StringBuffer::buffer_size = sb.buffer_size;
    StringBuffer::str_length = sb.str_length;
}

StringBuffer::~StringBuffer() {
    delete[] StringBuffer::str;
}

StringBuffer& StringBuffer::operator=(const StringBuffer &sb) {
    if (this != &sb) {
        delete[] StringBuffer::str;
        StringBuffer::str = new char[sb.buffer_size];
        memcpy(StringBuffer::str, sb.str, sb.str_length);
        StringBuffer::buffer_size = sb.buffer_size;
        StringBuffer::str_length = sb.str_length;
    }
    return *this;
}

size_t StringBuffer::size() {
    return StringBuffer::str_length;
}

size_t StringBuffer::capacity() {
    return StringBuffer::buffer_size;
}

void StringBuffer::clear() {
    memcpy(StringBuffer::str, "\0", sizeof(char));
    StringBuffer::str_length = 0;
}

void StringBuffer::insert(const char *str, size_t pos) {
    if (str != nullptr) {
        if (pos + strlen(str) > StringBuffer::buffer_size) {
            char* tmp = new char[this->str_length];
            memcpy(tmp, this->str, this->str_length);
            delete[] StringBuffer::str;
            StringBuffer::str = new char[pos + strlen(str) + 2];
            StringBuffer::buffer_size = pos + strlen(str) + 2;
            memcpy(this->str, tmp, strlen(tmp));
            delete[] tmp;
        }
        if (pos > StringBuffer::str_length) {
            for (int i = 0; i < pos - StringBuffer::str_length; i++) {
                memcpy(StringBuffer::str + StringBuffer::str_length + i, " ", sizeof(char));
            }
            StringBuffer::str_length = pos + strlen(str);
        }
        memcpy(StringBuffer::str + pos, str, strlen(str));
        memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));

    }
}

void StringBuffer::insert(const StringBuffer &sb, size_t pos) {
    if (pos + sb.str_length > StringBuffer::buffer_size) {
        char* tmp = new char[this->str_length];
        memcpy(tmp, this->str, this->str_length);
        delete[] StringBuffer::str;
        StringBuffer::str = new char[pos + sb.str_length + 2];
        StringBuffer::buffer_size = pos + sb.str_length + 2;
        memcpy(this->str, tmp, strlen(tmp));
        delete[] tmp;
    }
    if (pos > StringBuffer::str_length) {
        for (int i = 0; i < pos - StringBuffer::str_length; i++) {
            memcpy(StringBuffer::str + StringBuffer::str_length + i, " ", sizeof(char));
        }
        StringBuffer::str_length = pos + sb.str_length;
    }
    memcpy(StringBuffer::str + pos, sb.str, sb.str_length);
    memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));
}

void StringBuffer::append(const char *str) {
    if (str != nullptr) {
        if (StringBuffer::str_length + strlen(str) > StringBuffer::buffer_size) {
            char* tmp = new char[this->str_length];
            memcpy(tmp, this->str, this->str_length);
            delete[] StringBuffer::str;
            StringBuffer::str = new char[StringBuffer::str_length + strlen(str) + 2];
            StringBuffer::buffer_size = StringBuffer::str_length + strlen(str) + 2;
            memcpy(this->str, tmp, strlen(tmp));
            delete[] tmp;
        }
        memcpy(StringBuffer::str + StringBuffer::str_length, str, strlen(str));
        StringBuffer::str_length += strlen(str);
        memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));
    }
}

void StringBuffer::append(const StringBuffer &sb) {
    if (StringBuffer::str_length + sb.str_length > StringBuffer::buffer_size) {
        char* tmp = new char[this->str_length];
        memcpy(tmp, this->str, this->str_length);
        delete[] StringBuffer::str;
        StringBuffer::str = new char[StringBuffer::str_length + sb.str_length + 2];
        StringBuffer::buffer_size = StringBuffer::str_length + sb.str_length + 2;
        memcpy(this->str, tmp, strlen(tmp));
        delete[] tmp;
    }
    memcpy(StringBuffer::str + StringBuffer::str_length, sb.str, sb.str_length);
    StringBuffer::str_length += sb.str_length;
    memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));
}

const char* StringBuffer::c_str() {
    return StringBuffer::str;
}

void StringBuffer::set(const char *str) {
    if (str != nullptr) {
        if (strlen(str) > StringBuffer::buffer_size) {
            delete[] StringBuffer::str;
            StringBuffer::str = new char[strlen(str) + 2];
            StringBuffer::buffer_size = strlen(str) + 2;
        }
        memcpy(StringBuffer::str, str, strlen(str));
        StringBuffer::str_length = strlen(str);
        memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));
    }
}

void StringBuffer::set(const StringBuffer &sb) {
    if (sb.str_length > StringBuffer::buffer_size) {
        delete[] StringBuffer::str;
        StringBuffer::str = new char[sb.str_length];
        StringBuffer::buffer_size = sb.str_length;
    }
    memcpy(StringBuffer::str, sb.str, sb.str_length);
    StringBuffer::str_length = sb.str_length;
    memcpy(StringBuffer::str + StringBuffer::str_length, "\0", sizeof(char));
}