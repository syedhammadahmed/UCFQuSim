//
// Created by hammad on 9/27/19.
//

#ifndef UCFQUSIM_NOT_H
#define UCFQUSIM_NOT_H

#include "QuGate.h"

class Not : public QuGate {

public:
    Not();

    Not(int depth, int cardinality);

    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;


};



#endif //UCFQUSIM_NOT_H
