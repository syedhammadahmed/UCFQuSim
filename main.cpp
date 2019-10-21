#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "core/gates/CNot.h"
#include "core/gates/QuGate.h"
#include "QuArchitecture.h"
#include "core/QuSimulator.h"
#include "core/gates/Hadamard.h"
#include "core/gates/Not.h"

using namespace std;

int sigma(int n){
    if(n>0)
        return n + sigma(n-1);
    return 0;
}
int main() {
    int i = 1;
    for (int i = 1; i < 20; i++) {
        cout << "n = " << i << " k = " << (i * (i + 1)) / 2 << " k/2 = " << (i * (i + 1)) / 4 << endl;
    }
    return 0;
}
/*
int main() {
    const int QU_BITS = 6;
    const int MAX_DEPTH = 10;

    string inputFileName = "sample.qasm";
    string inputDirectory(getenv("HOME"));
    inputDirectory += "/input/";
    cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architecture(16);
    architecture.addConstraint(1,2);
    architecture.addConstraint(2,3);
    architecture.addConstraint(3,14);
    architecture.addConstraint(15,14);
    architecture.addConstraint(15,0);
    architecture.addConstraint(0,1);
    cout << "Architecture constraints: " <<  endl << architecture;

//    QuCircuit circuit(QU_BITS, MAX_DEPTH);
//    circuit.add(new Hadamard(), 0, 0);
//    circuit.add(new Not(), 0, 1);
//    circuit.add(new Hadamard(), 0, 2);
//    circuit.add(new Not(), 0, 7);
//    circuit.add(new Hadamard(), 1, 0);
//    circuit.add(new Hadamard(), 4, 4);
//    circuit.add(new Hadamard(), 0, 8);
//    cout << "Circuit: " <<  endl << circuit;

      QuCircuit circuit(inputDirectory + inputFileName, architecture.getN()); // input circuit file, # of physical qubits (max logical qubits)
//    circuit.add(new Hadamard(), 0, 0);
//    circuit.add(new Not(), 0, 1);
//    circuit.add(new Hadamard(), 0, 2);
//    circuit.add(new Not(), 0, 7);
//    circuit.add(new Hadamard(), 1, 0);
//    circuit.add(new Hadamard(), 4, 4);
//    circuit.add(new Hadamard(), 0, 8);
//    cout << "Circuit: " <<  endl << circuit;
//
    QuSimulator simulator(QU_BITS, MAX_DEPTH, circuit, architecture);
    simulator.run();


    return 0;
}
*/