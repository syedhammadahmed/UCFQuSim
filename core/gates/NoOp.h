//
// Created by YUSHAH on 10/16/2019.
//

#ifndef UCFQUSIM_NOOP_H
#define UCFQUSIM_NOOP_H


#include "QuGate.h"

class NoOp : public QuGate {

    NoOp();

    NoOp(int depth, int cardinality);

    // TODO: generate constructors
    void apply(QuBit *bit, int i) override;

};


#endif //UCFQUSIM_NOOP_H
