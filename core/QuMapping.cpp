//
// Created by SHA on 11/18/19.
//

#include <algorithm>

#include "util/Util.h"
#include "util/Constants.h"
#include "QuMapping.h"
#include "QuArchitecture.h"
using namespace std;

QuMapping::QuMapping(int n) : n(n) {
    init(Constants::INIT_MAPPING_DEFAULT);
}

QuMapping::QuMapping(const QuMapping& arg):n(arg.n) {
    mappingId = arg.mappingId;
    parentMappingId = arg.parentMappingId;
    clearSwapInstructions();
    setSwapInstructions(arg.swapInstructions);
    physicalToLogical.clear();
    for(int i=0; i<n; i++){
        physicalToLogical.push_back(arg.physicalToLogical[i]);
    }
}

QuMapping& QuMapping::operator=(const QuMapping &arg) {
    n = arg.n;
    mappingId = arg.mappingId;
    parentMappingId = arg.parentMappingId;
    clearSwapInstructions();
    setSwapInstructions(arg.swapInstructions);
    physicalToLogical.clear();
    for(int i=0; i<n; i++){
        physicalToLogical.push_back(arg.physicalToLogical[i]);
    }
    return (*this);
}

bool QuMapping::operator==(const QuMapping &arg) {
    for(int i=0; i<n; i++) {
        if(physicalToLogical[i] != arg.physicalToLogical[i])
            return false;
    }
    return true;
}

int& QuMapping::operator[](int index) {
    return physicalToLogical[index];
}

void QuMapping::init(vector<int> initSequence) {
    init(Constants::INIT_MAPPING_NO_MAPPING);
    for(int i=0; i<initSequence.size(); i++) {
        physicalToLogical[i] = initSequence[i]; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
    }
    setUnallocatedQuBits();
}

void QuMapping::init(int initializingPolicy) {  // 0 = default
    switch (initializingPolicy) {
        case Constants::INIT_MAPPING_DEFAULT: defaultInit(); break;
        case Constants::INIT_MAPPING_HARD_CODED: hardCodedInit(); break; // see Util::makeMappingVector()
        case Constants::INIT_MAPPING_NO_MAPPING: noMappingInit(); break;
    }
}

int QuMapping::getPhysicalBit(int logicalBit){
    for(int i=0; i<n; i++){
        if(physicalToLogical[i] == logicalBit)
            return i;
    }
    return -1;
}

void QuMapping::quSwapPhysical(int i, int j) {
    int temp = physicalToLogical[i];
    physicalToLogical[i] = physicalToLogical[j];
    physicalToLogical[j] = temp;
}

void QuMapping::quSwapLogical(int i, int j) {
    int pi = getPhysicalBit(i);
    int pj = getPhysicalBit(j);
    quSwapPhysical(pi, pj);
}

vector<Swap> QuMapping::fixMappings(std::vector<int> swapSeq) {
    vector<Swap> swapGates;
    this->print();
    int swapSeqSize = swapSeq.size();
    for(int i=0; i<signed(swapSeqSize-1); i++){
        quSwapPhysical(swapSeq[i], swapSeq[i + 1]);
        Swap swapGate;
        swapGate.setArgAtIndex(0, physicalToLogical[swapSeq[i]]); // todo src and target check acc. to arch.
        swapGate.setArgAtIndex(1, physicalToLogical[swapSeq[i+1]]);
        swapGates.push_back(swapGate);
        Util::println("SWAP " + to_string(physicalToLogical[swapSeq[i]]) + ", " + to_string(physicalToLogical[swapSeq[i+1]]));
    }
    return swapGates;
}

void QuMapping::print() {
//    if(Util::verbose) {
        for (int i = 0; i < n; i++) {
            cout << "Q" << i << " -> q" << physicalToLogical[i] << endl;
        }
//    }
}

void QuMapping::printShort() {
    cout << toString() << endl;
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

const string QuMapping::getMappingId() const {
    return mappingId;
}

void QuMapping::setMappingId(const string &mappingId) {
    this->mappingId = mappingId;
}

const string QuMapping::getParentMappingId() const {
    return parentMappingId;
}

void QuMapping::setParentMappingId(const string &parentMappingId) {
    this->parentMappingId = parentMappingId;
}

void QuMapping::setSwapInstructions(const vector<Swap> swapInstructions) {
    for (int i = 0; i < swapInstructions.size(); ++i) {
        Swap swapGate;
        swapGate.setArgAtIndex(0, swapInstructions[i].getArgAtIndex(0));
        swapGate.setArgAtIndex(1, swapInstructions[i].getArgAtIndex(1));
        this->swapInstructions.push_back(swapGate);
    }
}

void QuMapping::clearSwapInstructions() {
    swapInstructions.clear();
}

const vector<Swap> QuMapping::getSwapInstructions() const {
    return swapInstructions;
}

void QuMapping::setValueAt(int index, int value) {
    if(index >= 0 && index < n)
        physicalToLogical[index] = value;
}

void QuMapping::defaultInit() {
    // default initial mapping
//    physicalToLogical.resize(n);
    physicalToLogical.clear();
    for(int i=0; i<n; i++) {
        physicalToLogical.push_back(i % n); // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
//        physicalToLogical[i] = (i) % n; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
    }
}

void QuMapping::noMappingInit() {
    // default initial mapping
    physicalToLogical.resize(n);
    for(int i=0; i<n; i++) {
        physicalToLogical[i] = -1;
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

void QuMapping::setPhysicalToLogical(const vector<int> &physicalToLogical) {
    this->physicalToLogical = physicalToLogical;
}

void QuMapping::hardCodedInit() {
    setPhysicalToLogical(Util::makeMappingVector());
}

QuMapping::QuMapping() {

}

