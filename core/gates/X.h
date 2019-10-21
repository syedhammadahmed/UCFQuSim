//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_X_H
#define UCFQUSIM_X_H


#include "QuGate.h"

class X : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_X_H
