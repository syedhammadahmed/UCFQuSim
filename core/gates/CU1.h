//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_CU1_H
#define UCFQUSIM_CU1_H

#include "QuGate.h"

class CU1 : public QuGate {

public:

    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_CU1_H
