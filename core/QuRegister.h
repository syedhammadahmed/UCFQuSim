//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUREGISTER_H
#define UCFQUSIM_QUREGISTER_H

#include "QuBit.h"

class QuRegister{
private:
    QuBit* alpha; // amplitudes
    int n; // # of qubits
//    int size; // 2^n i.e. the array size of the register


public:

    QuRegister();
    QuRegister(int n); // create n-qubit register
    QuRegister(const QuRegister&);


    bool isValid();

    void print();

    ~QuRegister();

};


#endif //UCFQUSIM_QUREGISTER_H
