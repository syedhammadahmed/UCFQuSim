//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUSMARTSWAPPER_H
#define UCFQUSIM_QUSMARTSWAPPER_H


#include "QuSwapStrategy.h"

class QuSmartSwapper: public QuSwapStrategy {
    vector<vector<QuMapping>> instructionWiseMappings;  // instructionWiseMappings[0] = mappings for instruction 0 (1st instruction)

    int findSwaps(QuGate *quGate, int **couplingMap, QuCircuit *circuit);
    void findAllMappings(int src, int dest, QuCircuit *circuit);
};


#endif //UCFQUSIM_QUSMARTSWAPPER_H
