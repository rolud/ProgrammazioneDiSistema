//
// Created by rolud on 27/05/2020.
//

#ifndef STUDIES_CONSTANT42_H
#define STUDIES_CONSTANT42_H

#include "Amount.h"

class Constant42 : public Amount<Constant42> {
public:
    double getValue() const {
        return 42;
    }
};

#endif //STUDIES_CONSTANT42_H
