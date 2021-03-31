//
// Created by SHA on 10/22/2019.
//

#ifndef UCFQUSIM_QUCIRCUITGENERATOR_H
#define UCFQUSIM_QUCIRCUITGENERATOR_H


#include "QuCircuit.h"

class QuCircuitGenerator {
private:
    QuCircuit circuit;

    shared_ptr<QuGate>** grid;
    int** simpleGrid;
    int n; // # of physical qubits
    int cols;

    int layer;
    int* quBitRecentLayer;

    string inputFileAbsPath;
    string header;

public:
    QuCircuitGenerator(int n, string inputFileAbsPath);
    ~QuCircuitGenerator();

    void buildFromFile(string fileName); // then call this; this creates grid as well as vector of instructions
    void buildGrid();
    void initGrids();
    void deleteGrids();
    void add(shared_ptr<QuGate> gate, int depth);
    void addSimple(shared_ptr<QuGate> gate, int depth, int instructionNo);
    int getLayerForNewGate(vector<int> quBits, int operands);

    void makeProgramFile(string outputFileName);
    bool somethingInBetween(int row1, int row2, int layer);
    bool somethingInBetween(vector<int> quBits, int operands, int layer);

    int getLayer() const;
    QuCircuit& getCircuit();

};


#endif //UCFQUSIM_QUCIRCUITGENERATOR_H
