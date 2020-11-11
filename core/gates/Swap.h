//
// Created by SHA on 9/26/19.
//

#ifndef UCFQUSIM_SWAP_H
#define UCFQUSIM_SWAP_H


#include "QuGate.h"

class Swap : public QuGate {
public:
    Swap();
    explicit Swap(const Swap& arg);

private:
    void apply(QuBit *bit, int i) override;
};


#endif //UCFQUSIM_SWAP_H
