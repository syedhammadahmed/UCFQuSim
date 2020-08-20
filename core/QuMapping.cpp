//
// Created by hammad on 11/18/19.
//

#include <cstdlib>
#include <util/Util.h>
#include <core/gates/QuGateFactory.h>
#include "QuMapping.h"
#include "QuArchitecture.h"

const int QuMapping::DEFAULT = 0;

//todo uncomment to restore dynamic array logic
//QuMapping::QuMapping(int n) : physicalToLogical(NULL), n(n) {
//    physicalToLogical = new int[n];
//    init(0);
//}
//
//QuMapping::QuMapping(const QuMapping& arg):physicalToLogical(NULL), n(arg.n) {
//    physicalToLogical = new int[n];
//    for(int i=0; i<n; i++){
//        physicalToLogical[i] = arg.physicalToLogical[i];
//    }
//}
//

//QuMapping::~QuMapping() {
//    delete [] physicalToLogical;
//}

QuMapping::QuMapping(int n) : n(n) {
    init(0);
    //    srand(time(NULL));
//    init(rand() % 3628800);
//    cout << "QuMapping Constructor OK!" << endl;
}


QuMapping::QuMapping(const QuMapping& arg):n(arg.n) {
    mappingId = arg.mappingId;
    parentMappingId = arg.parentMappingId;
    swapInstructions = arg.swapInstructions;
    for(int i=0; i<n; i++){
        physicalToLogical[i] = arg.physicalToLogical[i];
    }
}

void QuMapping::init(int initializingPolicy) {  // 0 = default
    if(initializingPolicy == 0){
    // default initial mapping
        for(int i=0; i<n; i++) {
            physicalToLogical[i] = (i) % n; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
        }

    }
    else {
        for(int i=0; i<n; i++) {
            physicalToLogical[i] = i; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
        }
//        string str = "012";
     string str = "0123456789";
        int n = str.size();
        vector<string> perms;
        Util::permute(str, 0, n-1, perms);
//        for (string perm: perms) {
            for(int i=0; i<perms[initializingPolicy].length(); i++){
                int val = perms[initializingPolicy][i] - 48;
//                cout << val << " ";
                physicalToLogical[i] = val;
            }
//            cout << endl;
//        }

        //    int k = 1;
//    quBitConfiguration[0] = 0;
//    for(int i = 0; k < rows && i < rows; i++)
//        for(int j = i + 1; k < rows && j < rows; j++)
//            if((quBitConfiguration[k] == -1) && ((couplingMap[i][j] == 1) || (couplingMap[i][j] == -1)))
//                quBitConfiguration[k++] = j;
    }
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

//void QuMapping::quSwap(int i, int j) {
//    int temp = physicalToLogical[getPhysicalBit(i)];
//    physicalToLogical[getPhysicalBit(i)] = physicalToLogical[getPhysicalBit(j)];
//    physicalToLogical[getPhysicalBit(j)] = temp;
//}

void QuMapping::fixMappings(int src, std::vector<int> swapSeq) {
    if(swapSeq.empty())
        return;
//    quSwap(src, swapSeq[0]);
    Util::println("SWAPPING: START");
    for(int i=0; i<signed(swapSeq.size()-2); i++){
        quSwap(swapSeq[i], swapSeq[i+1]);
    }
    Util::println("SWAPPING: END");
}

vector<QuGate*> QuMapping::fixMappings(std::vector<int> swapSeq) {
    vector<QuGate*> swapGates;
    int swapSeqSize = swapSeq.size();
    for(int i=0; i<signed(swapSeqSize-1); i++){
        quSwap(swapSeq[i], swapSeq[i+1]);
        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
        swapGate->getArgIndex()[0] = physicalToLogical[swapSeq[i]];
        swapGate->getArgIndex()[1] = physicalToLogical[swapSeq[i+1]];
        swapGates.push_back(swapGate);
        Util::println("SWAP " + to_string(physicalToLogical[swapSeq[i]]) + ", " + to_string(physicalToLogical[swapSeq[i+1]]));
    }
    return swapGates;
}

void QuMapping::print() {
    if(Util::verbose) {
        for (int i = 0; i < n; i++) {
            cout << "Q" << i << " -> q" << physicalToLogical[i] << endl;
        }
    }
}

void QuMapping::quSwapLogical(int i, int j) {
    int pi = getPhysicalBit(i);
    int pj = getPhysicalBit(j);
    int temp = physicalToLogical[pi];
    physicalToLogical[pi] = physicalToLogical[pj];
    physicalToLogical[pj] = temp;
}

QuMapping::QuMapping(int n, int permId) : n(n) {
    init(permId);
//    cout << "QuMapping Constructor OK!" << endl;
}

const string &QuMapping::getMappingId() const {
    return mappingId;
}

void QuMapping::setMappingId(const string &mappingId) {
    this->mappingId = mappingId;
}

const string &QuMapping::getParentMappingId() const {
    return parentMappingId;
}

void QuMapping::setParentMappingId(const string &parentMappingId) {
    this->parentMappingId = parentMappingId;
}

vector<QuGate*>& QuMapping::getSwapInstructions(){
return swapInstructions;
}

void QuMapping::setSwapInstructions(const vector<QuGate*> swapInstructions) {
    this->swapInstructions = swapInstructions;
}

void QuMapping::operator=(const QuMapping &arg) {
    n = arg.n;
    mappingId = arg.mappingId;
    parentMappingId = arg.parentMappingId;
    swapInstructions = arg.swapInstructions;
    for(int i=0; i<n; i++){
        physicalToLogical[i] = arg.physicalToLogical[i];
    }
}

bool QuMapping::operator==(const QuMapping &arg) {
    for(int i=0; i<n; i++) {
        if(physicalToLogical[i] != arg.physicalToLogical[i])
            return false;
    }
    return true;
}

QuMapping::~QuMapping() {
    Util::println("~QuMapping() begin");
    if(!destructorCalled) {
        for (int i = 0; i < swapInstructions.size(); i++) {
            delete swapInstructions[i];
        }
    }
    destructorCalled = true;
    Util::println("~QuMapping() end");
}

void QuMapping::clearSwapInstructions() {
    this->swapInstructions.clear();
}


QuMapping::QuMapping() = default;
