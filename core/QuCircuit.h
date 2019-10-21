//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUCIRCUIT_H
#define UCFQUSIM_QUCIRCUIT_H

#include <vector>
#include <ostream>
#include <string>
#include "gates/QuGate.h"

using namespace std;

class QuCircuit {

private:
    int rows; // # of physical quBits (max rows)
    int cols; // depth
    int* quBitConfiguration; // logical positions of quBits (as they may change due to swap)
    int* quBitMapping; // logical to physical mapping of quBits
    int* quBitLastLayer; // the last layer # where any gate is using this qubit
    QuBit* quBits;
    QuGate*** grid;

public:
    QuCircuit();
    QuCircuit(string fileName, int rows);
    QuCircuit(int rows, int cols);

    void add(QuGate* gate, int row, int depth);
    void addMapping(int logicalQuBit, int physicalQuBit);
    void run();
//    QuGate* operator[][](int, int);

    friend std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit);

    virtual ~QuCircuit();

    void init1();
    void init2();

    void setGateQubits(QuGate *pGate, int pInt[3]);
};


#endif //UCFQUSIM_QUCIRCUIT_H
