//
// Created by SHA on 11/18/19.
//

#include <cstdlib>
#include <util/Util.h>
#include <core/gates/QuGateFactory.h>
#include <core/generator/QuMappingInitializer.h>
#include <algorithm>

#include "QuMapping.h"
#include "QuArchitecture.h"
using namespace std;


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
    clearSwapInstructions();
    setSwapInstructions(arg.swapInstructions);
    for(int i=0; i<n; i++){
        physicalToLogical[i] = arg.physicalToLogical[i];
    }
}


void QuMapping::init(int initializingPolicy) {  // 0 = default
    defaultInit();
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

vector<Swap> QuMapping::fixMappings(std::vector<int> swapSeq) {
    vector<Swap> swapGates;
    int swapSeqSize = swapSeq.size();
    for(int i=0; i<signed(swapSeqSize-1); i++){
        quSwap(swapSeq[i], swapSeq[i+1]);
        Swap swapGate;
        swapGate.setArgAtIndex(0, physicalToLogical[swapSeq[i]]); // todo src and target check acc. to arch.
        swapGate.setArgAtIndex(1, physicalToLogical[swapSeq[i+1]]);
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

string QuMapping::toString() {
    string mappingStr = "";
    for (int i = 0; i < n; i++) {
        mappingStr += to_string(i) + "->" + to_string(physicalToLogical[i]);
        if(i<n-1)
            mappingStr += ", ";
    }
    return mappingStr;
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


//vector<QuGate*> QuMapping::getSwapInstructions(){
//return swapInstructions;
//}
//
void QuMapping::setSwapInstructions(const vector<Swap> &swapInstructions) {
    for (int i = 0; i < swapInstructions.size(); ++i) {
        Swap swapGate;
        swapGate.setArgAtIndex(0, swapInstructions[i].getArgAtIndex(0));
        swapGate.setArgAtIndex(1, swapInstructions[i].getArgAtIndex(1));
        this->swapInstructions.push_back(swapGate);

    }
}


//void QuMapping::setSwapInstructions(const vector<QuGate*>& swapInstructions) {
//    for(QuGate* gate: swapInstructions) {
//        QuGate *swapGate = QuGateFactory::getQuGate("SWAP");
//        swapGate->getArgIndex()[0] = gate->getArgIndex()[0];
//        swapGate->getArgIndex()[1] = gate->getArgIndex()[1];
//        this->swapInstructions.push_back(swapGate);
//    }
//}

void QuMapping::operator=(const QuMapping &arg) {
    n = arg.n;
    mappingId = arg.mappingId;
    parentMappingId = arg.parentMappingId;
    clearSwapInstructions();
    setSwapInstructions(arg.swapInstructions);
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
//    for(int i=0; i<swapInstructions.size(); i++) {
//        delete swapInstructions[i];
//    }

}

void QuMapping::clearSwapInstructions() {
    swapInstructions.clear();
}

const vector<Swap> &QuMapping::getSwapInstructions() const {
    return swapInstructions;
}

//void QuMapping::setSwapInstructions(const vector<Swap> &swapInstructions) {
//    for (int i = 0; i < swapInstructions.size(); ++i) {
//        this->swapInstructions.push_back(swapInstructions[i]);
//    }
//}

const int *QuMapping::getPhysicalToLogical() const {
    return physicalToLogical;
}

void QuMapping::setValueAt(int index, int value) {
    if(index >= 0 && index < n)
        physicalToLogical[index] = value;
}

QuMapping::QuMapping(string mappingId) {
    this->mappingId = mappingId;
}

QuMapping::QuMapping(bool doStrongInit) {
    if (doStrongInit)
        strongInit();
    else
        defaultInit();
}

void QuMapping::defaultInit() {
    // default initial mapping
    for(int i=0; i<n; i++) {
        physicalToLogical[i] = (i) % n; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
    }
}

void QuMapping::strongInit() {
    // default initial mapping
    for(int i=0; i<n; i++) {
        physicalToLogical[i] = -1;
    }
}

void QuMapping::init(vector<int> initSequence) {
    defaultInit();
    for(int i=0; i<initSequence.size(); i++) {
        physicalToLogical[i] = initSequence[i]; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
    }
}

void QuMapping::setValueAtNextFree(int i) {
    for (int j = 0; j < n; ++j) {
        if(physicalToLogical[j] == -1) {
            physicalToLogical[j] = i;
            break;
        }
    }
}

int QuMapping::getValueAt(int i) {
    return physicalToLogical[i];
}

void QuMapping::setUnallocatedQuBits() {
    vector<int> remaining;
    for (int j = 0; j < n; ++j) {
        remaining.push_back(j);
    }
    for (int j = 0; j < n; ++j) {
        if (physicalToLogical[j] != -1)
            remaining.erase(remove(remaining.begin(), remaining.end(), physicalToLogical[j]), remaining.end());
    }

    for (int j = 0; j < n; ++j) {
        if (physicalToLogical[j] == -1) {
            physicalToLogical[j] = remaining[0];
            remaining.erase(remaining.begin());
        }
    }
}

void QuMapping::setN(int n) {
    this->n = n;
}

bool QuMapping::isLegit() {
    for (int j = 0; j < n; ++j)
        if (physicalToLogical[j] == -1)
            return false;
    return true;
}

int QuMapping::getN() const {
    return n;
}