//
// Created by hammad on 9/26/19.
//

#include "QuGate.h"

QuGate::QuGate() : depth(0), isElementary(false), cardinality(1), symbol('@') {
}
QuGate::QuGate(int depth) : depth(depth), isElementary(false), cardinality(1) {
}
QuGate::QuGate(int depth, bool isElementary) : depth(depth), isElementary(isElementary), cardinality(1) {
}
QuGate::QuGate(int depth, bool isElementary, int cardinality) : depth(depth), isElementary(isElementary), cardinality(cardinality) {
}
QuGate::QuGate(int depth, int cardinality, char symbol) : depth(depth), cardinality(cardinality), symbol(symbol) {
}

QuBit& QuGate::operator[](int i) {
    return *arg[i];
}

QuGate::~QuGate() {
}

char QuGate::getSymbol() const {
    return symbol;
}

