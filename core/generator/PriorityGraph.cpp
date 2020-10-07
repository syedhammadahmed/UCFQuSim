//
// Created by hammad on 9/9/20.
//

#include <algorithm>
#include "PriorityGraph.h"

void PriorityGraph::caterNode(int quBit, bool isSrc) {
    if (!isPresent(quBit)) {
        PriorityNode newNode(quBit, -1, (int) !isSrc, (int) isSrc);
        quBitNodes.push_back(newNode);
    }
    else{
        for (auto& quBitNode: quBitNodes) {
            if (quBitNode.getLogicalQubit() == quBit){
                isSrc ? quBitNode.incOutDegree() : quBitNode.incInDegree();
                break;
            }
        }
    }
}

bool PriorityGraph::isPresent(int quBit){
    for (auto& quBitNode: quBitNodes) {
        if (quBitNode.getLogicalQubit() == quBit)
            return true;
    }
    return false;
}

void PriorityGraph::sortByRank() {
    sort(quBitNodes.begin(), quBitNodes.end(), PriorityGraph::sortByRankDesc);
}

bool PriorityGraph::sortByRankDesc(const PriorityNode &a, const PriorityNode &b){
    return (a.getRank() > b.getRank());
}

int PriorityGraph::getSize() {
    return quBitNodes.size();
}

PriorityNode &PriorityGraph::getNode(int i) {
    return quBitNodes[i];
}

PriorityNode PriorityGraph::getHead() {
    PriorityNode temp;
    if (!quBitNodes.empty()){
        temp = quBitNodes[0];
        quBitNodes.erase(quBitNodes.begin());
    }
    return temp;
}
