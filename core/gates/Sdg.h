//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_SDG_H
#define UCFQUSIM_SDG_H

#include "QuGate.h"

class Sdg : public QuGate {

public:
    void apply(QuBit *bit, int i) override;
};

#endif //UCFQUSIM_SDG_H
