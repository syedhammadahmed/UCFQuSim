//
// Created by SHA on 9/9/20.
//

#ifndef UCFQUSIM_PRIORITYGRAPH_H
#define UCFQUSIM_PRIORITYGRAPH_H

#include <vector>
#include "PriorityNode.h"

using namespace std;

class PriorityGraph {
private:
    vector<PriorityNode> quBitNodes;

public:
    void caterNode(int quBit, bool isSrc);  // add node if not made, else update node info

    bool isPresent(int quBit);

    void sortByRank();
    static bool sortByRankDesc(const PriorityNode &a, const PriorityNode &b);

    int getSize();

    PriorityNode getHead();

    PriorityNode &getNode(int i);


};


#endif //UCFQUSIM_PRIORITYGRAPH_H
