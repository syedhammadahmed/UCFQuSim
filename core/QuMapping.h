//
// Created by SHA on 11/18/19.
//

#ifndef UCFQUSIM_QUMAPPING_H
#define UCFQUSIM_QUMAPPING_H

#include <vector>
#include <iostream>
#include <memory>

#include "core/gates/QuGate.h"
#include "core/gates/Swap.h"
#include "QuArchitecture.h"

using namespace std;

class QuMapping {
private:
    vector<int> physicalToLogical;
    int n;
    string mappingId;
    string parentMappingId;
    vector<Swap> swapInstructions;

public:
    QuMapping();
    explicit QuMapping(int n);
    QuMapping(const QuMapping& arg);
    ~QuMapping();
    QuMapping& operator=(const QuMapping& arg);
    bool operator==(const QuMapping& arg);
    int& operator[](int index);

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

    const vector<Swap> getSwapInstructions() const;

//    void setSwapInstructions(const vector<Swap> &swapInstructions);

    const vector<int> getPhysicalToLogical() const;
    void setValueAt(int index, int value);
    int getValueAt(int index);

    void defaultInit();

    void init(vector<int> initSequence);

    void noMappingInit();

    void setValueAtNextFree(int i);

    void setUnallocatedQuBits();

    void setN(int n);
    int getN() const;

    bool isLegit();

    string toString();

    void setPhysicalToLogical(const vector<int> &physicalToLogical);

    QuMapping(string mappingId);

    void hardCodedInit();

    void setSwapInstructions(const vector<Swap> swapInstructions);
};


#endif //UCFQUSIM_QUMAPPING_H
