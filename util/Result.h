//
// Created by YUSHAH on 4/28/2020.
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
    int timeProposed;

public:
    Result(const string &file, int swaps, int gatesOriginal, int gatesProposed, int depthProposed,
           int timeProposed);

    void print();
    static void printHeader();
};


#endif //UCFQUSIM_RESULT_H
