//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_Z_H
#define UCFQUSIM_Z_H

#include "QuGate.h"

class Z : public QuGate {

public:
    Z();

    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_Z_H
