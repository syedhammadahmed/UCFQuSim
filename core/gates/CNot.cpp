//
// Created by SHA on 9/26/19.
//

#include "CNot.h"

//CNot::CNot(int depth, int cardinality) : QuGate(depth, cardinality) {
//
//
//}

void CNot::apply(QuBit *bit, int i) {

}

CNot::CNot() : QuGate(2, "S", "cx"){
    printSymbol[0] = "S";
    printSymbol[1] = "T";
    printIndex = 0;
}

