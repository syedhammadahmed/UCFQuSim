//
// Created by SHA on 4/28/2020.
//

#include <iomanip>
#include "Result.h"

void Result::print() {
    cout << std::setw(20) << std::left << file << "\t";
    cout << std::setw(20) << std::left << n << "\t";
    cout << std::setw(20) << std::left << gatesOriginal << "\t";
//    cout << std::setw(20) << std::left << swaps << "\t";
//    cout << std::setw(20) << std::left << hadamards << "\t";
    cout << std::setw(20) << std::left << gatesProposed << "\t";
    cout << std::setw(20) << std::left << gatesProposedOptimized << "\t";
    cout << std::setw(20) << std::left << depthProposed + 1 << "\t";
    cout << std::setw(20) << std::left << timeProposed << "\t";
    cout << std::setw(20) << std::left << endl;
}
void Result::printHeader() {
    cout << std::setw(20) << std::left << "File"  << "\t";
    cout << std::setw(20) << std::left << "n"  << "\t";
    cout << std::setw(20) << std::left << "g"  << "\t";
//    cout << std::setw(20) << std::left << "s"  << "\t";
//    cout << std::setw(20) << std::left << "h"  << "\t";
    cout << std::setw(20) << std::left << "g*"  << "\t";
    cout << std::setw(20) << std::left << "g+"  << "\t";
    cout << std::setw(20) << std::left << "d*"  << "\t";
    cout << std::setw(20) << std::left << "t*" << "\t";
    cout << endl;
}

Result::Result(const string &file, int swaps, int gatesOriginal, int gatesProposed,
               int depthProposed, int hadamards, double timeProposed, int n) : file(file), swaps(swaps), gatesOriginal(gatesOriginal), gatesProposed(gatesProposed),
                                                      depthProposed(depthProposed), hadamards(hadamards), timeProposed(timeProposed), n(n) {}
Result::Result(const string &file, int swaps, int gatesOriginal, int gatesProposed,
               int depthProposed, int hadamards, double timeProposed, int gatesProposedOptimized, int n) : file(file), swaps(swaps), gatesOriginal(gatesOriginal), gatesProposed(gatesProposed),
                                                      depthProposed(depthProposed), hadamards(hadamards), timeProposed(timeProposed), gatesProposedOptimized(gatesProposedOptimized), n(n) {}

string Result::getFile() {
    return file;
}


