//
// Created by hammad on 11/18/19.
//

#ifndef UCFQUSIM_QUMAPPING_H
#define UCFQUSIM_QUMAPPING_H


#include <vector>
#include <iostream>
#include <core/gates/QuGate.h>
#include <memory>
#include <core/gates/Swap.h>
#include "QuArchitecture.h"

using namespace std;

class QuMapping {
private:
//    int* physicalToLogical;  // todo revert to dynamic logic
    int physicalToLogical[16];
    int n;
    string mappingId;
    string parentMappingId;
    vector<Swap> swapInstructions;
//    vector<unique_ptr<QuGate>> swapInstructions;
    bool destructorCalled;

public:
    int getN() const;

    static const int DEFAULT;
    explicit QuMapping(int n);
    explicit QuMapping(int n, int permId);

    QuMapping(bool strongInit = false);

    QuMapping(const QuMapping& arg);
    void operator=(const QuMapping& arg);
    bool operator==(const QuMapping& arg);

    virtual ~QuMapping();

//    virtual ~QuMapping();

    int getLogicalMapping(int physicalBit);
    void setLogicalMapping(int physicalBit, int logicalBit);

    void init(int initializingPolicy);

    int getPhysicalBit(int logicalBit);

    void quSwap(int i, int j);
    void quSwapLogical(int i, int j);

    void fixMappings(int src, std::vector<int> swapSeq);
    vector<Swap> fixMappings(std::vector<int> swapSeq);
    void print();

    const string &getMappingId() const;

    void setMappingId(const string &mappingId);

    const string &getParentMappingId() const;

    void setParentMappingId(const string &parentMappingId);

//    vector<Swap> getSwapInstructions();
//
//    void setSwapInstructions(const vector<Swap>& swapInstructions);
    void clearSwapInstructions();

    const vector<Swap> &getSwapInstructions() const;

    void setSwapInstructions(const vector<Swap> &swapInstructions);

    const int *getPhysicalToLogical() const;
    void setValueAt(int index, int value);
    int getValueAt(int index);

    void defaultInit();

    void init(vector<int> initSequence);

    void strongInit();

    void setValueAtNextFree(int i);

    void setUnallocatedQuBits();

    void setN(int n);
    bool isLegit();
};


#endif //UCFQUSIM_QUMAPPING_H
