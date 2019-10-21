//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUGATE_H
#define UCFQUSIM_QUGATE_H

#include "../QuBit.h"

using namespace std;
// abstract super-class for all elementary and
class QuGate {

protected:
    int depth;
    bool isElementary;
    int cardinality; // # of qubits required to be applied
    QuBit* arg[3];
    std::string symbol;
    int** matrix;
    int* argIndex;
    std::string printSymbol[3];
    int printIndex;
    string mnemonic;

public:
    static const int MAX_OPERANDS = 3;

    QuGate();
//    QuGate(int depth); // default cardinality = 1, isElemenatary = false
//    QuGate(int depth, bool isElementary); // default cardinality = 1
//    QuGate(int depth, bool isElementary, int cardinality);

    QuGate(int cardinality, std::string symbol, string mnemonic);

    QuBit& operator[](int);

    virtual ~QuGate();

    virtual void apply(QuBit*, int) = 0;

    std::string getSymbol() const;

    int* getArgIndex() const;

    int getCardinality() const;

    int getPrintIndex() const;

    void setPrintIndex(int printIndex);

    std::string getPrintSymbol();
};


#endif //UCFQUSIM_QUGATE_H
