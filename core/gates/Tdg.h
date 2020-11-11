//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_TDG_H
#define UCFQUSIM_TDG_H


#include "QuGate.h"

class Tdg : public QuGate {

public:
    Tdg();

    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_TDG_H
