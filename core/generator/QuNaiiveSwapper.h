//
// Created by SHA on 11/19/19.
//

#ifndef UCFQUSIM_QUNAIIVESWAPPER_H
#define UCFQUSIM_QUNAIIVESWAPPER_H


#include "QuSwapStrategy.h"

class QuNaiiveSwapper: public QuSwapStrategy {
public:
    void findShortestPathsFor1InputMapping() override;
    int findTotalSwaps() override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

    QuNaiiveSwapper(QuCircuit &circuit, QuArchitecture& architecture);
};


#endif //UCFQUSIM_QUNAIIVESWAPPER_H
