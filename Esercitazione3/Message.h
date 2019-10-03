//
// Created by rolud on 25/09/2019.
//

#ifndef ESERCITAZIONE3_MESSAGE_H
#define ESERCITAZIONE3_MESSAGE_H


#include "Symbol.h"

class Message {
public:
    enum Action {INSERT,DELETE};

    Message(Symbol symbol, Action action);
    const Symbol &get_symbol() const;
    Action get_action() const;

private:
    Symbol _symbol;
    Action _action;
};


#endif //ESERCITAZIONE3_MESSAGE_H
