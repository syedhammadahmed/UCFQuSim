//
// Created by hammad on 9/26/19.
//

#include "Hadamard.h"

void Hadamard::apply(QuBit *bit, int i) {

}

//Hadamard::Hadamard(int depth, int cardinality) : QuGate(depth, cardinality, 'H') {}

//Hadamard::Hadamard() : QuGate(0, 0, 'H'){}

Hadamard::Hadamard() : QuGate(1, "H", "h"){}