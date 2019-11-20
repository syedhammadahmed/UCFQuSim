//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUNAIIVESWAPPER_H
#define UCFQUSIM_QUNAIIVESWAPPER_H


#include "QuSwapStrategy.h"

class QuNaiiveSwapper: public QuSwapStrategy {
public:
    int findSwaps(QuGate *quGate, int **couplingMap, QuCircuit* circuit) override;


};


#endif //UCFQUSIM_QUNAIIVESWAPPER_H
