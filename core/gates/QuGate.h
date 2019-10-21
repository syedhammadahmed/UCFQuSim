//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUGATE_H
#define UCFQUSIM_QUGATE_H

#include "../QuBit.h"

// abstract super-class for all elementary and
class QuGate {

protected:
    int depth;
    bool isElementary;
    int cardinality; // # of qubits required to be applied
    QuBit* arg[3];
    char symbol;
    int** matrix;

public:
    static const int MAX_OPERANDS = 3;

    QuGate();
    QuGate(int depth); // default cardinality = 1, isElemenatary = false
    QuGate(int depth, bool isElementary); // default cardinality = 1
    QuGate(int depth, bool isElementary, int cardinality);

    QuGate(int depth, int cardinality, char symbol);

    QuBit& operator[](int);

    virtual ~QuGate();

    virtual void apply(QuBit*, int) = 0;

    char getSymbol() const;

};


#endif //UCFQUSIM_QUGATE_H
