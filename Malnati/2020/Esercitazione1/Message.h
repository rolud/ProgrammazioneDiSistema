//
// Created by rolud on 24/05/2020.
//

#ifndef ESERCITAZIONE1_MESSAGE_H
#define ESERCITAZIONE1_MESSAGE_H

#include <iostream>
#include <string>

class Message {
    long id;
    char *data;
    int size;

    static long idCounter;
    static long allocationCounter;

    char* mkMessage(int n);
public:
    Message();
    Message(int size);
    Message(const Message &source);
    Message(Message &&source);
    ~Message();

    Message& operator = (const Message &source);
    Message& operator = (Message &&source);

    long getId() const;
    char* getData() const;
    int getSize() const;
    static long getAllocationCounter();


};

std::ostream& operator << (std::ostream &out, const Message &m);

#endif //ESERCITAZIONE1_MESSAGE_H
