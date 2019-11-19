//
// Created by hammad on 11/18/19.
//

#ifndef UCFQUSIM_QUMAPPING_H
#define UCFQUSIM_QUMAPPING_H


#include <vector>

class QuMapping {
    int* physicalToLogical;
    int n;

public:
    QuMapping(int n);
    virtual ~QuMapping();

    int getLogicalMapping(int physicalBit);
    void setLogicalMapping(int physicalBit, int logicalBit);

    void init(int **couplingMap);

    int getPhysicalBit(int logicalBit);

    void quSwap(int i, int j);

    void fixMappings(int src, std::vector<int> swapSeq);
};


#endif //UCFQUSIM_QUMAPPING_H
