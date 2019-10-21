//
// Created by hammad on 10/8/19.
//

#ifndef UCFQUSIM_QUGATEFACTORY_H
#define UCFQUSIM_QUGATEFACTORY_H


#include "gates/QuGate.h"
#include <string>

using namespace std;

class QuGateFactory {

public:
    static QuGate* getQuGate(string symbol, int args[]);
    static QuGate* getQuGate(string symbol);

};


#endif //UCFQUSIM_QUGATEFACTORY_H
