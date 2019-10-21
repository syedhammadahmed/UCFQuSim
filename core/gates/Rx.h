//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_RX_H
#define UCFQUSIM_RX_H

#include "QuGate.h"

class Rx : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_RX_H
