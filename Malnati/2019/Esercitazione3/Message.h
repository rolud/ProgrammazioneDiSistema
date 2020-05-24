//
// Created by rolud on 25/09/2019.
//

#ifndef ESERCITAZIONE3_MESSAGE_H
#define ESERCITAZIONE3_MESSAGE_H


#include "Symbol.h"

class Message {
public:
    enum Action {INSERT,DELETE};

    Message(Symbol symbol, Action action, int editor_id);
    const Symbol &get_symbol() const;
    Action get_action() const;
    int get_editor_id() const;

private:
    Symbol symbol_;
    Action action_;
    int editor_id_;
};



#endif //ESERCITAZIONE3_MESSAGE_H
