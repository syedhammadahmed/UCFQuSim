//
// Created by hammad on 9/27/19.
//

#include <iostream>
#include <util/Util.h>
#include "QuArchitecture.h"

QuArchitecture::QuArchitecture(int n) : n(n) {
    couplingMap = new int*[n];
    for(int i = 0; i < n; i++)
        couplingMap[i] = new int[n];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            couplingMap[i][j] = 0;
    addConstraintsQX5();
}

void QuArchitecture::addConstraint(int src, int dest) {
    couplingMap[src][dest] = 1;
    couplingMap[dest][src] = -1;
//    cout << src << " " << dest << " SSS " << endl;
//    cout << couplingMap[dest][src] << " SSS " << endl;
}

std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture) {
    for(int i = 0; i < architecture.n; i++)
        for(int j = 0; j < architecture.n; j++)
            if((i!=j) && (architecture.couplingMap[i][j] > 0))
                os << "Q" << i << " => Q" << j << std::endl;
//            else if(architecture.couplingMap[i][j] == -1)
//                os << "Q" << i << " <= Q" << j << std::endl;
    return os;
}

void QuArchitecture::printCouplingMatrix() {
    if(Util::verbose) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << couplingMap[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

QuArchitecture::~QuArchitecture() {
    for(int i=0; i<n; i++)
        delete [] couplingMap[i];
    delete [] couplingMap;
}

int QuArchitecture::getN() {
    return n;
}

int** QuArchitecture::getCouplingMap() const {
    return couplingMap;
}

void QuArchitecture::addConstraintsQX3() {
    QuArchitecture& architectureQX3 = *this;
    architectureQX3.addConstraint(1,2);
    architectureQX3.addConstraint(2,3);
    architectureQX3.addConstraint(3,14);
    architectureQX3.addConstraint(15,14);
    architectureQX3.addConstraint(15,0);
    architectureQX3.addConstraint(0,1);

    architectureQX3.addConstraint(4,3);
    architectureQX3.addConstraint(4,5);
    architectureQX3.addConstraint(12,5);
    architectureQX3.addConstraint(12,13);
    architectureQX3.addConstraint(13,4);
    architectureQX3.addConstraint(13,14);

    architectureQX3.addConstraint(12,11);
    architectureQX3.addConstraint(6,11);
    architectureQX3.addConstraint(6,7);
    architectureQX3.addConstraint(8,7);
    architectureQX3.addConstraint(9,8);
    architectureQX3.addConstraint(9,10);
    architectureQX3.addConstraint(7,10);
    architectureQX3.addConstraint(11,10);
}


void QuArchitecture::addConstraintsQX5() {
    QuArchitecture& architectureQX5 = *this;
    architectureQX5.addConstraint(1,2);
    architectureQX5.addConstraint(1,0);

    architectureQX5.addConstraint(2,3);
    architectureQX5.addConstraint(3,4);
    architectureQX5.addConstraint(3,14);
    architectureQX5.addConstraint(5,4);
    architectureQX5.addConstraint(6,5);
    architectureQX5.addConstraint(6,7);
    architectureQX5.addConstraint(6,11);

    architectureQX5.addConstraint(7,10);
    architectureQX5.addConstraint(8,7);

    architectureQX5.addConstraint(9,8);
    architectureQX5.addConstraint(9,10);

    architectureQX5.addConstraint(11,10);

    architectureQX5.addConstraint(12,5);
    architectureQX5.addConstraint(12,11);
    architectureQX5.addConstraint(12,13);

    architectureQX5.addConstraint(13,4);
    architectureQX5.addConstraint(13,14);

    architectureQX5.addConstraint(15,0);
    architectureQX5.addConstraint(15,2);
    architectureQX5.addConstraint(15,14);
}

bool QuArchitecture::isAdjacent(int src, int dest) {
    if(couplingMap[src][dest] != 0)
        return true;
    return false;
}

bool QuArchitecture::isCompatable(int src, int dest) {
    if(couplingMap[src][dest] == 1)
        return true;
    return false;
}