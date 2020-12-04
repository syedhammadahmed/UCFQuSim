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
    static int performCNOTCancellations(vector<QuGate *> &finalProgram);
    static int performUnaryCancellations(vector<QuGate *> &finalProgram);
private:
    static bool isClearInBetween(int left, int right, vector<QuGate*>& finalProgram);
    static bool areCancellable(int left, int right, vector<QuGate*>& finalProgram);

};

#endif //UCFQUSIM_QUCIRCUITOPTIMIZER_H
