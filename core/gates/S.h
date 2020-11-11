//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_S_H
#define UCFQUSIM_S_H

#include "QuGate.h"

class S : public QuGate {

public:
    void apply(QuBit *bit, int i) override;

    S();
};

#endif //UCFQUSIM_S_H
