//
// Created by rolud on 27/05/2020.
//

#ifndef STUDIES_AMOUNT_H
#define STUDIES_AMOUNT_H

template <typename T> class Amount {
    Amount() = default;
    friend T;
public:
    double getValue() const {
        return static_cast<T const&>(*this).getValue();
    }
};

#endif //STUDIES_AMOUNT_H
