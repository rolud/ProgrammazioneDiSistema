//
// Created by rolud on 25/09/2019.
//

#include "Symbol.h"

Symbol::Symbol(std::string id, char value, std::vector<int> position) : _id(id), _value(value), _position(position) {

}

const std::string &Symbol::get_id() const {
    return _id;
}

char Symbol::get_value() const {
    return _value;
}

const std::vector<int> &Symbol::get_position() const {
    return _position;
}
