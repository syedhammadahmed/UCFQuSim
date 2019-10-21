//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_IDENTITY_H
#define UCFQUSIM_IDENTITY_H


#include "QuGate.h"

class Identity : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_IDENTITY_H
