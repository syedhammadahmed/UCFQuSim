//
// Created by hammad on 9/27/19.
//

#ifndef UCFQUSIM_QUARCHITECTURE_H
#define UCFQUSIM_QUARCHITECTURE_H


#include <ostream>

class QuArchitecture {
private:
    int n; // # of physical quBits
    int** couplingMap;

public:
    QuArchitecture(int n);
    void addConstraint(int src, int dest);

    friend std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture);
    // adds a CNOT constraint from src qubit to dest qubit

    int getN();

    virtual ~QuArchitecture();
};


#endif //UCFQUSIM_QUARCHITECTURE_H
