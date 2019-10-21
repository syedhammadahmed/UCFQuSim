//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_CHADAMARD_H
#define UCFQUSIM_CHADAMARD_H


#include "QuGate.h"

class CHadamard : public QuGate {

public:
    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_CHADAMARD_H
