//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_Y_H
#define UCFQUSIM_Y_H

#include "QuGate.h"

class Y : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_Y_H
