//
// Created by SHA on 10/8/19.
//

#ifndef UCFQUSIM_T_H
#define UCFQUSIM_T_H

#include "QuGate.h"

class T : public QuGate {

public:
    T();

    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_T_H
