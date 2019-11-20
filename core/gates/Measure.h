//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_MEASURE_H
#define UCFQUSIM_MEASURE_H


#include "QuGate.h"

class Measure : public QuGate {

public:
    void apply(QuBit *bit, int i) override;

    Measure();
};


#endif //UCFQUSIM_MEASURE_H
