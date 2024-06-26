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
    int n;

public:
    Result(const string &file, int swaps, int gatesOriginal, int gatesProposed, int depthProposed, int hadamards,
           double timeProposed, int n);
    Result(const string &file, int swaps, int gatesOriginal, int gatesProposed, int depthProposed, int hadamards,
           double timeProposed, int gatesProposedOptimized, int n);

    void print();
    static void printHeader();

    string getFile();
};


#endif //UCFQUSIM_RESULT_H
