//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_CRZ_H
#define UCFQUSIM_CRZ_H

#include "QuGate.h"

class CRz : public QuGate {

public:
    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;

    CRz();
};



#endif //UCFQUSIM_CRZ_H
