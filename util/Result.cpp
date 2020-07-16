//
// Created by YUSHAH on 4/28/2020.
//

#include "Result.h"

void Result::print() {
    cout << file << "\t";
    cout << gatesOriginal << "\t";
    cout << swaps << "\t";
    cout << swaps * 7 << "\t";
    cout << gatesProposed << "\t";
    cout << depthProposed + 1 << "\t";
    cout << timeProposed << "\t";
    cout << endl;
}

void Result::printHeader() {
    cout << "File\t";
    cout << "g\t";
    cout << "s\t";
    cout << "s*7\t";
    cout << "g*\t";
    cout << "d*\t";
    cout << "t*\t";
    cout << endl;
}

Result::Result(const string &file, int swaps, int gatesOriginal, int gatesProposed,
               int depthProposed, double timeProposed) : file(file), swaps(swaps), gatesOriginal(gatesOriginal), gatesProposed(gatesProposed),
                                                      depthProposed(depthProposed), timeProposed(timeProposed) {}


