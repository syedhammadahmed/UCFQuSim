//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUSWAPSTRATEGY_H
#define UCFQUSIM_QUSWAPSTRATEGY_H


#include <core/gates/QuGate.h>
#include "QuCircuit.h"

class QuSwapStrategy {
protected:
    QuCircuit& circuit;
    int programCounter;
    vector<int> swapPath;
    vector<vector<int>> allSPFSwapPaths;
public:
    virtual int findSwapsFor1Instruction(QuGate *quGate, int **couplingMap) = 0;
    virtual int findTotalSwaps(QuArchitecture& quArchitecture) = 0;
    QuSwapStrategy(QuCircuit& circuit);
    virtual vector<int> swapAlongPath(int* parent, int source, int destination) = 0;
    virtual QuMapping getCurrentMapping() = 0;
};


#endif //UCFQUSIM_QUSWAPSTRATEGY_H
