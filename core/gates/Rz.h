//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_RZ_H
#define UCFQUSIM_RZ_H

#include "QuGate.h"

class Rz : public QuGate {

public:
    void apply(QuBit *bit, int i) override;

    Rz();
};

#endif //UCFQUSIM_RZ_H
