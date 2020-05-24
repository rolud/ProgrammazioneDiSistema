//
// Created by rolud on 24/05/2020.
//

#ifndef STUDIES_BUFFER_H
#define STUDIES_BUFFER_H


class Buffer {

public:
    Buffer(int size);
    ~Buffer();

    int getSize();
    bool getValue(int pos, int &val);
    bool setValue(int pos, int val);

private:
    int size;
    int* buffer;
};


#endif //STUDIES_BUFFER_H
