//
// Created by hammad on 9/26/19.
//

#include "QuGate.h"

QuGate::QuGate() : depth(0), isElementary(false), cardinality(1), symbol("@"), printIndex(0) {
    argIndex = new int[cardinality];
}
//QuGate::QuGate(int depth) : depth(depth), isElementary(false), cardinality(1) {
//}
//QuGate::QuGate(int depth, bool isElementary) : depth(depth), isElementary(isElementary), cardinality(1) {
//}
//QuGate::QuGate(int depth, bool isElementary, int cardinality) : depth(depth), isElementary(isElementary), cardinality(cardinality) {
//}
QuGate::QuGate(int cardinality, string symbol, string mnemonic) : cardinality(cardinality), symbol(symbol), printIndex(0), mnemonic(mnemonic) {
    argIndex = new int[cardinality];
}

QuBit& QuGate::operator[](int i) {
    return *arg[i];
}

QuGate::~QuGate() {
    delete [] argIndex;
}

std::string QuGate::getSymbol() const {
    return symbol;
}

int* QuGate::getArgIndex() const {
    return argIndex;
}

int QuGate::getCardinality() const {
    return cardinality;
}

int QuGate::getPrintIndex() const {
    return printIndex;
}

void QuGate::setPrintIndex(int printIndex) {
    QuGate::printIndex = printIndex;
}


std::string QuGate::getPrintSymbol() {
    if (printIndex >= 1)
        return printSymbol[printIndex];
    return symbol;
}

