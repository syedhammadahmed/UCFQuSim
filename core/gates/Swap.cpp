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

