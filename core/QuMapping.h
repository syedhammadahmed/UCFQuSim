//
// Created by hammad on 11/18/19.
//

#ifndef UCFQUSIM_QUMAPPING_H
#define UCFQUSIM_QUMAPPING_H


#include <vector>
#include <iostream>
#include "QuArchitecture.h"

using namespace std;



class QuMapping {

private:
//    int* physicalToLogical;  // todo revert to dynamic logic
    int physicalToLogical[16];
    int n;

public:
    static const int DEFAULT;
    explicit QuMapping(int n);
    explicit QuMapping(int n, int permId);

    QuMapping();

    QuMapping(const QuMapping& arg);

//    virtual ~QuMapping();

    int getLogicalMapping(int physicalBit);
    void setLogicalMapping(int physicalBit, int logicalBit);

    void init(int initializingPolicy);

    int getPhysicalBit(int logicalBit);

    void quSwap(int i, int j);
    void quSwapLogical(int i, int j);

    void fixMappings(int src, std::vector<int> swapSeq);
    void fixMappings(std::vector<int> swapSeq);
    void print();

    void setMapping(const QuMapping& arg);
};


#endif //UCFQUSIM_QUMAPPING_H
