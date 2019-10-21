//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUCIRCUIT_H
#define UCFQUSIM_QUCIRCUIT_H

#include <vector>
#include <ostream>
#include <string>
#include "gates/QuGate.h"
#include "QuInstruction.h"

using namespace std;

class QuCircuit {

private:

    int rows; // # of physical quBits (max rows)
    int cols; // depth
    int* logicalToPhysicalMapping; // logical to physical mapping of quBits : logical index -> physical elements
    int* physicalToLogicalMapping; // physical to logical mapping of quBits : physical index -> logical elements
    int* quBitRecentLayer;
    QuBit* logicalQuBits;
    QuGate*** grid;

    vector<QuGate*> instructions;


public:
    QuCircuit();
    QuCircuit(string fileName, int rows);
    QuCircuit(int rows, int cols);

    void add(QuGate* gate, int row, int depth);
    void add(QuGate* gate, int depth);
    void addMapping(int logicalQuBit, int physicalQuBit);
    void run();
//    QuGate* operator[][](int, int);

    friend std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit);

    virtual ~QuCircuit();

    void init1();

    void init2();

    int getLayerForNewGate(int gates[3], int operands);

    bool somethingInBetween(int row1, int row2, int layer);

    void initQuBitMappings(int **couplingMap);

    int findSwapsFor1Instruction(QuGate* quGate, int **couplingMap);

    void initializeMappings();

    void initializeMappings(int **couplingMap);

    void printMappings();
    void printGrid();

    int findTotalSwaps(int **couplingMap);

    int swapAlongPath(int *parent, int source, int destination);
};


#endif //UCFQUSIM_QUCIRCUIT_H
