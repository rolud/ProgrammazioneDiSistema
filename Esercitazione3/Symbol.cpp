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

bool Symbol::operator<(const Symbol &s) {
    auto l = this->get_position();
    auto r = s.get_position();
    int min = std::min(l.size(), r.size());
    for (int i = 0; i < min; i++) {
        int l1 = l[i];
        int r1 = r[i];
        if (l1 < r1)
            return true;
    }
    if (l.size() < r.size())
        return true;
    return false;
}

bool Symbol::operator>(const Symbol &s) {
    auto l = this->get_position();
    auto r = s.get_position();
    int min = std::min(l.size(), r.size());
    for (int i = 0; i < min; i++) {
        int l1 = l[i];
        int r1 = r[i];
        if (l1 > r1)
            return true;
    }
    if (l.size() > r.size())
        return true;
    return false;
}
