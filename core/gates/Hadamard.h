//
// Created by SHA on 9/26/19.
//

#ifndef UCFQUSIM_HADAMARD_H
#define UCFQUSIM_HADAMARD_H

#include "QuGate.h"

class Hadamard : public QuGate {

public:
    Hadamard();

    Hadamard(int depth, int cardinality);


    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_HADAMARD_H
