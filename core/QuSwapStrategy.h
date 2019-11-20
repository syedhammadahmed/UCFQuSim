//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUSWAPSTRATEGY_H
#define UCFQUSIM_QUSWAPSTRATEGY_H


#include <core/gates/QuGate.h>
#include "QuCircuit.h"

class QuSwapStrategy {
public:
    virtual int findSwaps(QuGate *quGate, int **couplingMap, QuCircuit* circuit) = 0;

};


#endif //UCFQUSIM_QUSWAPSTRATEGY_H
