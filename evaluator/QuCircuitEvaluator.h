//
// Created by SHA on 10/22/2019.
//

#ifndef UCFQUSIM_QUCIRCUITEVALUATOR_H
#define UCFQUSIM_QUCIRCUITEVALUATOR_H


#include "../core/QuCircuit.h"

class QuCircuitEvaluator {
private:
    QuCircuit& circuit;
    QuMapping initialMapping;
    int* logicalToPhysicalMapping; // logical to physical mapping of quBits : logical index -> physical elements
    int* physicalToLogicalMapping; // physical to logical mapping of quBits : physical index -> logical elements

public:
    QuCircuitEvaluator(QuCircuit &circuit);
    QuCircuitEvaluator(QuCircuit &circuit, QuMapping& initialMapping);

    virtual ~QuCircuitEvaluator();

    bool evaluateCNOTConstraints(int** couplingMap);
    void initializeMappings(int** couplingMap);

    void printMappings();
};


#endif //UCFQUSIM_QUCIRCUITEVALUATOR_H
