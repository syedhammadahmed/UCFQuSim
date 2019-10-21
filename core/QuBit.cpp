//
// Created by hammad on 9/26/19.
//

#include "QuBit.h"



std::ostream &operator<<(std::ostream &os, const QuBit &bit) {
    os << "val: " << bit.amplitudes;
    return os;
}

QuBit::QuBit() {

}

QuBit::QuBit(int, int) {

}

void QuBit::init() {

}

int &QuBit::operator[](int i) {
    return amplitudes[i];
}

void QuBit::print() {

}

QuBit::~QuBit() {

}

int QuBit::measure() {
    int prAlpha = amplitudes[0] * amplitudes[0];
    int prBeta = amplitudes[1] * amplitudes[1];

    classicalBit = 1;
    if(prAlpha > prBeta)
        classicalBit = 0;

        return 0;
}
