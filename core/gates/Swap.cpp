//
// Created by hammad on 9/26/19.
//

#include "Swap.h"

void Swap::apply(QuBit *bit, int i) {

}
Swap::Swap() : QuGate(2, "><", "SWAP") {
    printSymbol[0] = "><";
    printSymbol[1] = "><";
    printIndex = 0;
}

Swap::Swap(const Swap &arg): QuGate(2, "><", "SWAP")  {
    for (int i = 0; i < arg.argIndex.size(); ++i) {
        argIndex[i] = arg.argIndex[i];
    }
}

