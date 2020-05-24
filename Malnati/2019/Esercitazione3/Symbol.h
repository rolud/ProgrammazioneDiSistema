//
// Created by rolud on 25/09/2019.
//

#ifndef ESERCITAZIONE3_SYMBOL_H
#define ESERCITAZIONE3_SYMBOL_H

#include <vector>
#include <string>

class Symbol {
public:
    Symbol(std::string id, char value, std::vector<int> position);

    const std::string &get_id() const;
    char get_value() const;
    const std::vector<int> &get_position() const;

    bool operator < (const Symbol &s);
    bool operator > (const Symbol &s);

private:
    std::string _id;
    char _value;
    std::vector<int> _position;
};


#endif //ESERCITAZIONE3_SYMBOL_H
