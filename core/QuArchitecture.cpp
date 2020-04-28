//
// Created by hammad on 9/27/19.
//

#include "QuArchitecture.h"

QuArchitecture::QuArchitecture(int n) : n(n) {
    couplingMap = new int*[n];
    for(int i = 0; i < n; i++)
        couplingMap[i] = new int[n];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            couplingMap[i][j] = 0;
    addConstraintsQX3();
}

void QuArchitecture::addConstraint(int src, int dest) {
    couplingMap[src][dest] = 1;
    couplingMap[dest][src] = -1;
}

std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture) {
    for(int i = 0; i < architecture.n; i++)
        for(int j = i + 1; j < architecture.n; j++)
            if(architecture.couplingMap[i][j] == 1)
                os << "Q" << i << " => Q" << j << std::endl;
            else if(architecture.couplingMap[i][j] == -1)
                os << "Q" << i << " <= Q" << j << std::endl;
    return os;
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
