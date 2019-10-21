//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_CNOT_H
#define UCFQUSIM_CNOT_H

#include "QuGate.h"

class CNot : public QuGate {

public:
    static const int CONTROL = 0;
    static const int TARGET = 1;

    CNot();
//    CNot(int depth, int cardinality);

    void apply(QuBit *bit, int i) override;

};


#endif //UCFQUSIM_CNOT_H
