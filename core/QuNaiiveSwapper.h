//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUNAIIVESWAPPER_H
#define UCFQUSIM_QUNAIIVESWAPPER_H


#include "QuSwapStrategy.h"

class QuNaiiveSwapper: public QuSwapStrategy {
public:
    int findSwapsFor1Instruction(QuGate *quGate, int **couplingMap) override;
    int findTotalSwaps(QuArchitecture& quArchitecture) override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

    QuNaiiveSwapper(QuCircuit &circuit);
};


#endif //UCFQUSIM_QUNAIIVESWAPPER_H
