//
// Created by SHA on 11/19/19.
//

#include "QuSwapStrategy.h"
#include "QuGateFactory.h"


QuSwapStrategy::QuSwapStrategy(QuCircuit &circuit, QuArchitecture& architecture): circuit(circuit), architecture(architecture), programCounter(0) {
//    cout << "QuSwapStrategy OK" << endl;
}

