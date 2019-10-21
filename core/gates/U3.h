//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_U3_H
#define UCFQUSIM_U3_H


#include "QuGate.h"

class U3 : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_U3_H
