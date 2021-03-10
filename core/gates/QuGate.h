//
// Created by SHA on 9/26/19.
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
    int gateId;
    bool cancelled;

public:
    static const int MAX_OPERANDS = 3;

    QuGate();
    QuGate(int cardinality, std::string symbol, string mnemonic);
    QuGate(int cardinality, std::string symbol, string mnemonic, int printIndex);

    QuBit& operator[](int);

    friend std::ostream &operator<<(std::ostream &os, const QuGate& quGate);

    virtual ~QuGate();
    virtual void apply(QuBit*, int) = 0;

    string getSymbol() const;
    int getCardinality() const;
    int getPrintIndex() const;
    void setPrintIndex(int printIndex);
    string getPrintSymbol();
    const string &getMnemonic() const;
    void setMnemonic(const string &mnemonic);
    bool isUnary();
    int getArgAtIndex(int index) const;
    void setArgAtIndex(int index, int val);
    const vector<int> &getArgIndex() const;
    void setArgIndex(const vector<int> &argIndex);
    string getTheta() const;
    void setTheta(string theta);
    int getGateId() const;
    void setGateId(int gateId);

    bool isCancelled() const;

    void setCancelled(bool cancelled);

    bool operator==(QuGate& arg);
    bool operator!=(QuGate& arg);
    bool isDitto(std::shared_ptr<QuGate> instruction);
    bool hasAnyOfArgs(const vector<int> argIndex);
};


#endif //UCFQUSIM_QUGATE_H
