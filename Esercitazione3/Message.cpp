//
// Created by rolud on 25/09/2019.
//

#include "Message.h"

Message::Message(Symbol symbol, Message::Action action, int editor_id): symbol_(symbol), action_(action), editor_id_(editor_id) {}

const Symbol &Message::get_symbol() const {
    return symbol_;
}

Message::Action Message::get_action() const {
    return action_;
}

int Message::get_editor_id() const {
    return editor_id_;
}