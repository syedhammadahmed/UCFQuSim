//
// Created by SHA on 9/25/19.
//

#ifndef UCFQUSIM_QUBIT_H
#define UCFQUSIM_QUBIT_H


#include <ostream>

class QuBit {

private:
    int amplitudes[2];
    int classicalBit;


public:
    static const int STATE_SUPERPOSITION = 0;
    static const int STATE_MEASURED = 1;
    static const int STATE_ENGTANGLED = 2;

    QuBit();
    QuBit(int, int);

    void init();
    int measure();

    int& operator[](int);


    void print();
    ~QuBit();

    friend std::ostream &operator<<(std::ostream &os, const QuBit &bit);


};


#endif //UCFQUSIM_QUBIT_H
