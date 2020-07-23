//
// Created by YUSHAH on 4/28/1515.
//

#include <iomanip>
#include "Result.h"

void Result::print() {
    cout << std::setw(15) << std::left << file << "\t";
    cout << std::setw(15) << std::left << gatesOriginal << "\t";
    cout << std::setw(15) << std::left << swaps << "\t";
    cout << std::setw(15) << std::left << swaps * 7 << "\t";
    cout << std::setw(15) << std::left << gatesProposed << "\t";
    cout << std::setw(15) << std::left << depthProposed + 1 << "\t";
    cout << std::setw(15) << std::left << timeProposed << "\t";
    cout << std::setw(15) << std::left << endl;
}

void Result::printHeader() {

    cout << std::setw(15) << std::left << "File"  << "\t";
    cout << std::setw(15) << std::left << "g"  << "\t";
    cout << std::setw(15) << std::left << "s"  << "\t";
    cout << std::setw(15) << std::left << "s*7"  << "\t";
    cout << std::setw(15) << std::left << "g*"  << "\t";
    cout << std::setw(15) << std::left << "d*"  << "\t";
    cout << std::setw(15) << std::left << "t*" << "\t";
    cout << endl;
}

Result::Result(const string &file, int swaps, int gatesOriginal, int gatesProposed,
               int depthProposed, double timeProposed) : file(file), swaps(swaps), gatesOriginal(gatesOriginal), gatesProposed(gatesProposed),
                                                      depthProposed(depthProposed), timeProposed(timeProposed) {}


