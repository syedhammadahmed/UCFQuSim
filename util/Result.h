//
// Created by SHA on 4/28/2020.
//

#ifndef UCFQUSIM_RESULT_H
#define UCFQUSIM_RESULT_H

#include <ostream>
#include <iostream>
#include <string>
#include "Result.h"
using namespace std;


class Result {
private:
    string file;
    int swaps;
    int gatesOriginal;
    int gatesProposed;
    int depthProposed;
    int hadamards;
    double timeProposed;
    int gatesProposedOptimized;

public:
    Result(const string &file, int swaps, int gatesOriginal, int gatesProposed, int depthProposed, int hadamards,
           double timeProposed);
    Result(const string &file, int swaps, int gatesOriginal, int gatesProposed, int depthProposed, int hadamards,
           double timeProposed, int gatesProposedOptimized);

    void print();
    static void printHeader();
};


#endif //UCFQUSIM_RESULT_H
