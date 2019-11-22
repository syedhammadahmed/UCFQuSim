//
// Created by hammad on 11/18/19.
//

#include <cstdlib>
#include "QuMapping.h"
#include "QuArchitecture.h"

QuMapping::QuMapping(int n) : physicalToLogical(NULL), n(n) {
    physicalToLogical = new int[n];
    init(0);
}

QuMapping::QuMapping(const QuMapping& arg):physicalToLogical(NULL), n(arg.n) {
    physicalToLogical = new int[n];
    for(int i=0; i<n; i++){
        physicalToLogical[i] = arg.physicalToLogical[i];
    }
}

QuMapping::~QuMapping() {
    delete [] physicalToLogical;
}

void QuMapping::init(int initializingPolicy) {  // 0 = default
    if(initializingPolicy == 0){
    // default initial mapping
        for(int i=0; i<n; i++) {
            physicalToLogical[i] = i; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
        }
    }
//    else {
//    int k = 1;
//    quBitConfiguration[0] = 0;
//    for(int i = 0; k < rows && i < rows; i++)
//        for(int j = i + 1; k < rows && j < rows; j++)
//            if((quBitConfiguration[k] == -1) && ((couplingMap[i][j] == 1) || (couplingMap[i][j] == -1)))
//                quBitConfiguration[k++] = j;
//    }
}

int QuMapping::getLogicalMapping(int physicalBit) {
    return physicalToLogical[physicalBit];
}

void QuMapping::setLogicalMapping(int physicalBit, int logicalBit) {
    physicalToLogical[physicalBit] = logicalBit;
}

int QuMapping::getPhysicalBit(int logicalBit){
    for(int i=0; i<n; i++){
        if(physicalToLogical[i] == logicalBit)
            return i;
    }
    return -1;
}

void QuMapping::quSwap(int i, int j) {
    int temp = physicalToLogical[i];
    physicalToLogical[i] = physicalToLogical[j];
    physicalToLogical[j] = temp;
}

void QuMapping::fixMappings(int src, std::vector<int> swapSeq) {
    if(swapSeq.empty())
        return;
//    quSwap(src, swapSeq[0]);
    for(int i=0; i<swapSeq.size()-2; i++){
        quSwap(swapSeq[i], swapSeq[i+1]);
    }
}

void QuMapping::print() {
    for(int i = 0; i < n; i++) {
        cout << "Q" << i << " -> q" << physicalToLogical[i] << endl;
    }
}
