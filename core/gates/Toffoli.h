//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_TOFFOLI_H
#define UCFQUSIM_TOFFOLI_H


#include "QuGate.h"

class Toffoli : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_TOFFOLI_H
