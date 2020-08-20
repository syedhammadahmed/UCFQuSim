//
// Created by YUSHAH on 10/22/2019.
//

#ifndef UCFQUSIM_QUCIRCUITGENERATOR_H
#define UCFQUSIM_QUCIRCUITGENERATOR_H


#include "QuCircuit.h"

class QuCircuitGenerator {
private:
//    QuCircuit& circuit;
    QuCircuit circuit;
    QuArchitecture& architecture;

    vector<QuGate*> instructions; // original qasm program instructions/qugates

    QuGate*** grid;
    int** simpleGrid;
    int rows;
    int cols;

    int layer;
    int* quBitRecentLayer;

    string inputFileAbsPath;

public:
//    QuCircuitGenerator(QuCircuit& circuit); // set rows of circuit and call this
//    QuCircuitGenerator(QuCircuit& circuit, QuArchitecture& architecture); // set rows of circuit and call this
    QuCircuitGenerator(QuArchitecture& architecture);
    QuCircuitGenerator(QuArchitecture& architecture, string inputFileAbsPath);
    ~QuCircuitGenerator();

    void removeUnaryInstructions();

    void buildFromFile(string fileName); // then call this; this creates grid as well as vector of instructions
    void buildGrid();
    void init2();
    void add(QuGate *gate, int depth);
    void addSimple(QuGate *gate, int depth, int instructionNo);
    int getLayerForNewGate(vector<int> quBits, int operands);

    void makeProgramFile(string outputFileName);
    bool somethingInBetween(int row1, int row2, int layer);
    bool somethingInBetween(vector<int> quBits, int operands, int layer);

    void setInstructions(const vector<QuGate*> instructions);
    int getLayer() const;
    const vector<QuGate*> getInstructions() const;
    QuCircuit& getCircuit();

    QuGate *parseInstruction(string line);
};


#endif //UCFQUSIM_QUCIRCUITGENERATOR_H
