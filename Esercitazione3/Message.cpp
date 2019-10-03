//
// Created by rolud on 25/09/2019.
//

#include "Message.h"

Message::Message(Symbol symbol, Message::Action action): _symbol(symbol), _action(action) {}

const Symbol &Message::get_symbol() const {
    return _symbol;
}

Message::Action Message::get_action() const {
    return _action;
}
