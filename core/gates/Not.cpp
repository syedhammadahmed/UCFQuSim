//
// Created by hammad on 9/27/19.
//

#include "Not.h"

Not::Not() : QuGate(0, 0, 'X'){
}

Not::Not(int depth, int cardinality)  : QuGate(depth, cardinality, 'X') {

}

void Not::apply(QuBit *bit, int i) {

}
