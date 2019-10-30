#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "core/gates/CNot.h"
#include "core/gates/QuGate.h"
#include "core/QuSimulator.h"
#include "core/gates/Hadamard.h"
#include "core/gates/Not.h"
#include "ShortestPathFinder.h"
#include "core/QuCircuitBuilder.h"
#include "evaluation/QuCircuitEvaluator.h"

using namespace std;

int main() {

    const int QU_BITS = 4;
    const int MAX_DEPTH = 10;

    string inputFileName = "sample.qasm";
//    getenv("HOME")
//    getenv("HOMEPATH")
//    string inputDirectory(string(getenv("HOMEDRIVE")) + getenv("HOMEPATH"));
//    string inputDirectory(string(getenv("HOME")));
    string inputDirectory("");
    inputDirectory += "../input/";
    cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architectureQX3(16);
    architectureQX3.addConstraint(1,2);
    architectureQX3.addConstraint(2,3);
    architectureQX3.addConstraint(3,14);
    architectureQX3.addConstraint(15,14);
    architectureQX3.addConstraint(15,0);
    architectureQX3.addConstraint(0,1);

    architectureQX3.addConstraint(4,3);
    architectureQX3.addConstraint(4,5);
    architectureQX3.addConstraint(12,5);
    architectureQX3.addConstraint(12,13);
    architectureQX3.addConstraint(13,4);
    architectureQX3.addConstraint(13,14);

    architectureQX3.addConstraint(12,11);
    architectureQX3.addConstraint(6,11);
    architectureQX3.addConstraint(6,7);
    architectureQX3.addConstraint(8,7);
    architectureQX3.addConstraint(9,8);
    architectureQX3.addConstraint(9,10);
    architectureQX3.addConstraint(7,10);
    architectureQX3.addConstraint(11,10);
//    cout << "architectureQX3 constraints: " <<  endl << architectureQX3;

//    QuArchitecture architectureQX3(4);
//    architectureQX3.addConstraint(0,1);
//    architectureQX3.addConstraint(1,2);
//    architectureQX3.addConstraint(2,3);
//    architectureQX3.addConstraint(1,3);
//    cout << "architectureQX3 constraints: " <<  endl << architectureQX3;

//    QuCircuit circuit(inputDirectory + inputFileName, architectureQX3.getN());
    QuCircuit circuit(architectureQX3.getN());
    QuCircuitBuilder quCircuitBuilder(circuit);
    quCircuitBuilder.buildFromFile(inputDirectory + inputFileName);
//    circuit.initializeMappings(architectureQX3.getCouplingMap());
    circuit.initializeMappings(NULL); // NULL is for trivial mapping x[i] = i
    int totalSwaps = circuit.findTotalSwaps(architectureQX3.getCouplingMap());
//    cout << "Circuit: " <<  endl << circuit;

    circuit.printInstructions();

    quCircuitBuilder.setInstructions(circuit.getInstructionsV1());
    quCircuitBuilder.makeProgramFile(inputDirectory + "output.qasm");

    cout << "Total Swaps Required: " << totalSwaps << endl;

    QuCircuit testCircuit(architectureQX3.getN());
    QuCircuitBuilder testQuCircuitBuilder(testCircuit);
    testQuCircuitBuilder.buildFromFile(inputDirectory + "output.qasm");
    QuCircuitEvaluator quCircuitEvaluator(testCircuit);
    bool satisfied = quCircuitEvaluator.evaluateCNOTConstraints(architectureQX3.getCouplingMap());
    if(satisfied)
        cout << "CNOT Constraints satisfied!" << endl;
    else
        cout << "CNOT Constraints NOT satisfied! Please make sure all non-unary gates have adjacent qubits!" << endl;
//    QuSimulator simulator(QU_BITS, MAX_DEPTH, circuit, architectureQX3);
//    simulator.run();

    return 0;
}