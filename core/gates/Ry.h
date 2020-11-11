//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_RY_H
#define UCFQUSIM_RY_H

#include "QuGate.h"

class Ry : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_RY_H
