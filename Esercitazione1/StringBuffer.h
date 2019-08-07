//
// Created by rolud on 07/08/2019.
//

#ifndef ESERCITAZIONE1_STRINGBUFFER_H
#define ESERCITAZIONE1_STRINGBUFFER_H

#include <string.h>

class StringBuffer {
public:
    StringBuffer();
    StringBuffer(const char* str);
    StringBuffer(const StringBuffer& sb);
    ~StringBuffer();
    StringBuffer& operator= (const StringBuffer& sb);
    size_t size();
    size_t capacity();
    void clear();
    void insert(const char* str, size_t pos);
    void insert(const StringBuffer& sb, size_t pos);
    void append(const char* str);
    void append(const StringBuffer& sb);
    const char* c_str();
    void set(const char* str);
    void set(const StringBuffer& sb);
private:
    char* str;
    size_t buffer_size;
    size_t str_length;
    const size_t BUFFERSIZE = 1024;
};


#endif //ESERCITAZIONE1_STRINGBUFFER_H
