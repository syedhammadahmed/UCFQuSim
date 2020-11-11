//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_CU3_H
#define UCFQUSIM_CU3_H


#include "QuGate.h"

class CU3 : public QuGate {

public:

    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_CU3_H
