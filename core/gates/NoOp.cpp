//
// Created by YUSHAH on 10/16/2019.
//

#include "NoOp.h"

NoOp::NoOp() : QuGate(1, "~", "NOOP"){}

NoOp::NoOp(int depth, int cardinality) {

}

void NoOp::apply(QuBit *bit, int i) {

}
