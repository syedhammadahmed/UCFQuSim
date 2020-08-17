//
// Created by hammad on 11/18/19.
//

#ifndef UCFQUSIM_QUMAPPING_H
#define UCFQUSIM_QUMAPPING_H


#include <vector>
#include <iostream>
#include <core/gates/QuGate.h>
#include "QuArchitecture.h"

using namespace std;

class QuMapping {
private:
//    int* physicalToLogical;  // todo revert to dynamic logic
    int physicalToLogical[16];
    int n;
    string mappingId;
    string parentMappingId;
    vector<QuGate*> swapInstructions;

public:
    static const int DEFAULT;
    explicit QuMapping(int n);
    explicit QuMapping(int n, int permId);

    explicit QuMapping();

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
    vector<QuGate*> fixMappings(std::vector<int> swapSeq);
    void print();

    const string &getMappingId() const;

    void setMappingId(const string &mappingId);

    const string &getParentMappingId() const;

    void setParentMappingId(const string &parentMappingId);

    vector<QuGate*>& getSwapInstructions();

    void setSwapInstructions(const vector<QuGate*> swapInstructions);
    void clearSwapInstructions();

};


#endif //UCFQUSIM_QUMAPPING_H
