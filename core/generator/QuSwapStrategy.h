//
// Created by SHA on 11/19/19.
//

#ifndef UCFQUSIM_QUSWAPSTRATEGY_H
#define UCFQUSIM_QUSWAPSTRATEGY_H


#include <core/gates/QuGate.h>
#include "QuCircuit.h"

class QuSwapStrategy {
protected:
    QuCircuit& circuit;
    QuArchitecture& architecture;
    int programCounter;
    vector<int> swapPath;
    vector<vector<int>> allSPFSwapPaths;
    QuGate* currentInstruction;
    unsigned int swaps;
public:
    virtual void findShortestPathsFor1InputMapping() = 0;
    virtual int findTotalSwaps() = 0;
    QuSwapStrategy(QuCircuit& circuit, QuArchitecture& architecture);
    virtual vector<int> swapAlongPath(int* parent, int source, int destination) = 0;
    virtual QuMapping getCurrentMapping() = 0;
//    virtual void setInitialMapping();
};


#endif //UCFQUSIM_QUSWAPSTRATEGY_H
