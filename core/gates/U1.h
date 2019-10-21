//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_U1_H
#define UCFQUSIM_U1_H

#include "QuGate.h"

class U1 : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_U1_H
