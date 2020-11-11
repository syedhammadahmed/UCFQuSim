//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_CZ_H
#define UCFQUSIM_CZ_H

#include "QuGate.h"

class CZ : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_CZ_H
