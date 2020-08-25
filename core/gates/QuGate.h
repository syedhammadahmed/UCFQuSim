//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUGATE_H
#define UCFQUSIM_QUGATE_H

#include <memory>
#include <vector>
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
    vector<int> argIndex;
    std::string printSymbol[3];
    int printIndex;
    string mnemonic;
    string theta;

public:
    static const int MAX_OPERANDS = 3;

    QuGate();
//    QuGate(int depth); // default cardinality = 1, isElemenatary = false
//    QuGate(int depth, bool isElementary); // default cardinality = 1
//    QuGate(int depth, bool isElementary, int cardinality);

    QuGate(int cardinality, std::string symbol, string mnemonic);
    QuGate(int cardinality, std::string symbol, string mnemonic, int printIndex);

    QuBit& operator[](int);

    virtual ~QuGate();
    friend std::ostream &operator<<(std::ostream &os, const QuGate& quGate);

    virtual void apply(QuBit*, int) = 0;

    std::string getSymbol() const;

    int getCardinality() const;

    int getPrintIndex() const;

    void setPrintIndex(int printIndex);

    std::string getPrintSymbol();

    const string &getMnemonic() const;

    void setMnemonic(const string &mnemonic);
    bool isUnary();

    int getArgAtIndex(int index) const;

    void setArgAtIndex(int index, int val);

    const vector<int> &getArgIndex() const;

    void setArgIndex(const vector<int> &argIndex);

    string getTheta() const;

    void setTheta(string theta);
};


#endif //UCFQUSIM_QUGATE_H
