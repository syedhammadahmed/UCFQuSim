//
// Created by YUSHAH on 10/22/2019.
//

#ifndef UCFQUSIM_QUCIRCUITBUILDER_H
#define UCFQUSIM_QUCIRCUITBUILDER_H


#include "QuCircuit.h"

class QuCircuitBuilder {
private:
    QuCircuit& circuit;
    int* quBitRecentLayer;
    vector<QuGate*> instructions; // original qasm program instructions/qugates
    QuGate*** grid;
    int cols;
    int rows;

public:
    QuCircuitBuilder(QuCircuit& circuit); // set rows of circuit and call this
    void buildFromFile(string fileName); // then call this; this creates grid as well as vector of instructions


    ~QuCircuitBuilder();

    void buildGrid();
    void init2();
    void add(QuGate *gate, int depth);
    int getLayerForNewGate(int *gates, int operands);

    void makeProgramFile(string outputFileName);

    void setInstructions(const vector<QuGate*> instructions);

    const vector<QuGate*> getInstructions() const;

    bool somethingInBetween(int row1, int row2, int layer);
};


#endif //UCFQUSIM_QUCIRCUITBUILDER_H
