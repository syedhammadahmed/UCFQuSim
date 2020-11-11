//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_X_H
#define UCFQUSIM_X_H


#include "QuGate.h"

class X : public QuGate {

public:
    void apply(QuBit *bit, int i) override;

    X();
};


#endif //UCFQUSIM_X_H
