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
    QuMapping& operator=(const QuMapping& arg);
    bool operator==(const QuMapping& arg);
    int& operator[](int index);

    void init(vector<int> initSequence);
    void init(int initializingPolicy);
    void defaultInit();
    void noMappingInit();
    void hardCodedInit();
    void setValueAtNextFree(int i);
    void setUnallocatedQuBits();
    bool isLegit(); // check if all allocated

    int getPhysicalBit(int logicalBit);
    void quSwapPhysical(int i, int j);
    void quSwapLogical(int i, int j);
    vector<Swap> fixMappings(vector<int> swapSeq);

    void clearSwapInstructions();

    void print();
    void printShort();
    string toString();

    const string getMappingId() const;
    void setMappingId(const string &mappingId);
    const string getParentMappingId() const;
    void setParentMappingId(const string &parentMappingId);
    const vector<Swap> getSwapInstructions() const;
    void setValueAt(int index, int value);
    int getValueAt(int index);
    void setN(int n);
    int getN() const;
    void setPhysicalToLogical(const vector<int> &physicalToLogical);
    void setSwapInstructions(const vector<Swap> swapInstructions);

};


#endif //UCFQUSIM_QUMAPPING_H
