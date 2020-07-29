#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <evaluator/QuMultiEvaluator.h>
#include <core/generator/QuMultiGenerator.h>
#include <iomanip>
#include <util/Result.h>
#include <util/Util.h>

#include "core/gates/CNot.h"
#include "core/gates/QuGate.h"
#include "core/QuSimulator.h"
#include "core/gates/Hadamard.h"
#include "core/gates/Not.h"
#include "ShortestPathFinder.h"
#include "core/generator/QuCircuitGenerator.h"
#include "AllShortestPathsFinder.h"

using namespace std;

int main() {
    const int quBits = 16;
    const int MAX_DEPTH = 10;
// hello
//    string inputFileName = "sample.qasm";
//    getenv("HOME")
//    getenv("HOMEPATH")
//    string inputDirectory(string(getenv("HOMEDRIVE")) + getenv("HOMEPATH"));
//    string inputDirectory(string(getenv("HOME")));
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";
//    inputDirectory += "/home/hammad/input/";
    cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
//    QuArchitecture architectureQX3(quBits); // includes the coupling map having CNOT constraints
//    cout << "architectureQX3 constraints: " <<  endl << architectureQX3;

    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
    vector<Result> results = quMultiGenerator.generateAllCircuits();
    Result::printHeader();
    for(Result result: results){
        result.print();
    }

//    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX3);
//    quMultiEvaluator.evaluateAllCircuits();

//    QuCircuit testCircuit(architectureQX3.getN());
//    QuCircuitGenerator testQuCircuitBuilder(testCircuit);
//    testQuCircuitBuilder.buildFromFile(inputDirectory + "output.qasm");
//    QuCircuitEvaluator quCircuitEvaluator(testCircuit);
//    bool satisfied = quCircuitEvaluator.evaluateCNOTConstraints(architectureQX3.getCouplingMap());
//    if(satisfied)
//        cout << "CNOT Constraints satisfied!" << endl;
//    else
//        cout << "CNOT Constraints NOT satisfied! Please make sure all non-unary gates have adjacent qubits!" << endl;

//    QuSimulator simulator(QU_BITS, MAX_DEPTH, circuit, architectureQX3);
//    simulator.run();

    return 0;
}

 /*
int main() {
    QuArchitecture architectureQX5(16); // includes the coupling map having CNOT constraints
    architectureQX5.printCouplingMatrix();
    AllShortestPathsFinder allSPF(architectureQX5.getCouplingMap(), architectureQX5.getN());

    for(int k=1; k<=8; k++) {
        for (int i = 0; i < architectureQX5.getN(); i++) {
            for (int j = 0; j < architectureQX5.getN(); j++) {
                if (i != j) {
                    vector<vector<int>> paths = allSPF.findSingleSourceAllShortestPaths(i, j, k);
                    cout << "(" << i << "," << j << "): [length = " << k << "]" << endl;
                    for (vector<int> path: paths) {
                        Util::printPath(path);
                    }
                    cout << endl;
                }
            }
        }
    }
//    int i = 0, j = 3, k = 1;
//    vector<vector<int>> paths = allSPF.findSingleSourceAllShortestPaths(i, j, k);
//    cout << "(" << i << "," << j << "): [length = " << k << "]" << endl;
//    for (vector<int> path: paths) {
//        Util::printPath(path);
//    }
//    cout << endl;
}
 */

// int main()
// {
////     string str = "012";
//     string str = "0123456789";
//     int n = str.size();
//     vector<string> perms;
//     Util::permute(str, 0, n-1, perms);
//     cout << perms.size();
////     for (string perm: perms) {
////         for(int i=0; i<perm.length(); i++){
////            int val = perm[i] - 48;
////            cout << val << " ";
////         }
////         cout << endl;
////     }
//     return 0;
// }
