//
// Created by rolud on 24/05/2020.
//

#include "Message.h"

long Message::idCounter = 0;
long Message::allocationCounter = 0;

char* Message::mkMessage(int n){
    std::string vowels = "aeiou";
    std::string consonants = "bcdfghlmnpqrstvz";
    char* m = new char[n+1];
    for(int i=0; i<n; i++){
        m[i]= i%2 ? vowels[rand()%vowels.size()] : consonants[rand()%consonants.size()];
    }
    m[n] = 0 ;
    return m;
}

Message::Message(): id(-1), data(nullptr), size(0){
    std::cout << "Message: default constructor" << std::endl;
    allocationCounter++;
}

Message::Message(int n): id(idCounter++), size(n){
    std::cout << "Message: constructor" << std::endl;
    this->data = mkMessage(n);
    allocationCounter++;
}

Message::Message(const Message& other){
    std::cout << "Message: copy constructor" << std::endl;
    this->id = other.id;
    this->size = other.size;
    this->data = new char[size+1];
    std::copy(other.data, other.data+other.size+1, this->data);
    allocationCounter++;
}

Message::Message(Message &&other){
    std::cout << "Message: move constructor" << std::endl;
    this->id = other.id;
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
    allocationCounter++;
}

Message& Message::operator=(const Message &other){
    std::cout << "Message: assign operator" << std::endl;
    if(this != &other){
        delete[] this->data;
        this->data = nullptr;
        this->id = other.id;
        this->size = other.size;
        this->data = new char[size+1];
        std::copy(other.data, other.data + other.size + 1, this->data);
    }
    return *this;
}

Message& Message::operator=(Message &&other) {
    std::cout << "Message: move assign operator" << std::endl;
    if(this != &other){
        delete[] this->data;
        this->id = other.id;
        this->size = other.size;
        this->data = other.data;
        other.data = nullptr;
    }
    return *this;
}

Message::~Message() {
    std::cout << "Message: destructor" << std::endl;
    delete [] data;
    allocationCounter--;
}

long Message::getId() const { return id; }

int Message::getSize() const { return size; }

char* Message::getData() const { return data; }

long Message::getAllocationCounter() { return allocationCounter; }

std::ostream& operator<<(std::ostream &out, const Message &m){
    std::string s(m.getData());
    out << "[id: " << m.getId() <<"] "
        << "[size: " << m.getSize() << "] "
        << "[data: " << s << "]";
    return out;
}