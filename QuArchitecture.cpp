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
}

void QuArchitecture::addConstraint(int src, int dest) {
    couplingMap[src][dest] = 1;
    couplingMap[dest][src] = -1;
}

std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture) {
    for(int i = 0; i < architecture.n; i++)
        for(int j = 0; j < architecture.n; j++)
            if(architecture.couplingMap[i][j] == 1)
                os << "Q" << i << " => Q" << j << std::endl;
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
