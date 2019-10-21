//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_CY_H
#define UCFQUSIM_CY_H

#include "QuGate.h"

class CY : public QuGate {

public:
    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_CY_H
