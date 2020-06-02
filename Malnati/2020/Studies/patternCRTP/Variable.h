//
// Created by rolud on 27/05/2020.
//

#ifndef STUDIES_VARIABLE_H
#define STUDIES_VARIABLE_H

#include "Amount.h"

class Variable : public Amount<Variable> {
public:
    double _value;
    double getValue() const {
        return _value;
    }
};

#endif //STUDIES_VARIABLE_H
