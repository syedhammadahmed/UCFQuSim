//
// Created by hammad on 9/9/20.
//

#include "PriorityNode.h"

PriorityNode::PriorityNode(int logicalQubit, int physicalQubit, int inDegree, int outDegree) : logicalQubit(
        logicalQubit), physicalQubit(physicalQubit), inDegree(inDegree), outDegree(outDegree), rank(this->inDegree + this->outDegree) {}

void PriorityNode::incOutDegree() {
    outDegree++;
    rank++;
}

void PriorityNode::incInDegree() {
    inDegree++;
    rank++;
}

PriorityNode::PriorityNode() : logicalQubit(-1), physicalQubit(-1), inDegree(-1), outDegree(-1), rank(-1) {}

int PriorityNode::getPhysicalQubit() const {
    return physicalQubit;
}

int PriorityNode::getInDegree() const {
    return inDegree;
}

int PriorityNode::getOutDegree() const {
    return outDegree;
}

int PriorityNode::getRank() const {
    return rank;
}

int PriorityNode::getLogicalQubit() const {
    return logicalQubit;
}

void PriorityNode::setPhysicalQubit(int physicalQubit) {
    PriorityNode::physicalQubit = physicalQubit;
}

