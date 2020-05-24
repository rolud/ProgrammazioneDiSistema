//
// Created by rolud on 24/05/2020.
//

#include "MessageStore.h"

MessageStore::MessageStore(int n) : n(n), dim(n){
    this->messages = new Message[n];
}

MessageStore::~MessageStore() {
    delete[] this->messages;
}

void MessageStore::add(Message m) {
    int posFree = -1;
    for(int i = 0; i < dim; i++) {
        if (this->messages[i].getId() == -1 && posFree == -1) posFree = i;
        if (this->messages[i].getId() == m.getId()) {
            this->messages[i] = m;
            return;
        }
    }

    // if there are not messages with same id,
    // m added at first free position found
    if (posFree != -1) {
        this->messages[posFree] = m;
        return;
    }

    // if there are not free positions, reallocate
    Message *newMessages = new Message[this->dim + n];
    for (int i = 0; i < this->dim; i++) {
        newMessages[i] = this->messages[i];
    }
    newMessages[dim] = m;
    this->dim += n;

    delete[] this->messages;
    this->messages = nullptr;
    this->messages = newMessages;

}

std::optional<Message> MessageStore::get(long id) {
    for (int i = 0; i < this->dim; i++)
        if (this->messages[i].getId() == id) return this->messages[i];
    return std::nullopt;
}

bool MessageStore::remove(long id) {
    for (int i = 0; i < this->dim; i++)
        if (this->messages[i].getId() == id) {
            this->messages[i] = Message();
            return true;
        }
    return false;
}

std::tuple<int, int> MessageStore::stats() {
    int valid = 0;
    int available = 0;
    for (int i = 0; i < this->dim; i++) {
        if (this->messages[i].getId() == -1)
            available++;
        else
            valid++;
    }
    return std::tuple<int,int> { valid, available };
}

void MessageStore::compact() {
    int messageCount = 0;
    for (int i = 0; i < dim; i++) {
        if (messages[i].getId() >= 0) messageCount++;
    }
    int newDim = messageCount % n == 0 ? messageCount : (1+ messageCount/n)*n;
    Message *newMessages = new Message[newDim];
    for (int i = 0, j = 0; i < dim; i++) {
        if (messages[i].getId() >= 0) {
            newMessages[j] = messages[i];
            j++;
        }
    }
    delete[] messages;
    dim = newDim;
    messages = newMessages;
}
