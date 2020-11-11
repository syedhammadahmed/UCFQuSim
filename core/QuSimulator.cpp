//
// Created by SHA on 9/26/19.
//

#include "QuSimulator.h"
#include "QuArchitecture.h"

QuSimulator::QuSimulator(int logicalBits, int depth, QuCircuit& circuit, QuArchitecture& architecture): circuit(circuit), architecture(architecture) {

}

void QuSimulator::run() {
    circuit.run();
}

//QuSimulator::QuSimulator(int logicalBits, int depth) {
//
//}
