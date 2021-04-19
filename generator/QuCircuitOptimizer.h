//
// Created by YUSHAH on 12/4/2020.
//

#ifndef UCFQUSIM_QUCIRCUITOPTIMIZER_H
#define UCFQUSIM_QUCIRCUITOPTIMIZER_H

#include "gates/QuGate.h"
#include <vector>
#include <iostream>
using namespace std;

class QuCircuitOptimizer {
public:
    static int performCNOTCancellations(int n, vector<shared_ptr<QuGate>>& finalProgram);
    static int performUnaryCancellations(int n, vector<shared_ptr<QuGate>>& finalProgram);
private:
    static shared_ptr<QuGate>** grid;
    static bool isClearInBetween(int left, int right, vector<shared_ptr<QuGate>>& finalProgram);
    static bool areCancellable(int left, int right, vector<shared_ptr<QuGate>>& finalProgram);

};

#endif //UCFQUSIM_QUCIRCUITOPTIMIZER_H
