//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUSIMULATOR_H
#define UCFQUSIM_QUSIMULATOR_H


#include "QuCircuit.h"
#include "QuArchitecture.h"

// will primarily  generate  and run the circuit
// also will contain the logical to physical qubit mappings
// also will initialize the system i.e. qubits
// will contain an instance of QuCircuit
class QuSimulator {

private:
    QuCircuit& circuit;
    QuArchitecture& architecture;

public:
//    QuSimulator(int logicalBits, int depth);
    QuSimulator(int logicalBits, int depth, QuCircuit& circuit, QuArchitecture& architecture);
    void run();


};


#endif //UCFQUSIM_QUSIMULATOR_H
