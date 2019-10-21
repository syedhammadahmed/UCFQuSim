//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_U2_H
#define UCFQUSIM_U2_H


#include "QuGate.h"

class U2 : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_U2_H
