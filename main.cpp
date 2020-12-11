#include <iostream>
#include <iomanip>
#include <string>

#include "evaluator/QuMultiEvaluator.h"
#include "core/generator/QuMultiGenerator.h"
#include "util/Result.h"
#include "util/Util.h"
#include "AllShortestPathsFinder.h"
#include "core/generator/QuSmartSwapper.h"
#include "core/generator/QuCircuitOptimizer.h"
#include "core/gates/QuGateFactory.h"

using namespace std;

int main() {
    const int quBits = 16;

//    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";
    if (Util::verbose)
        cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX5);

    auto data = quMultiGenerator.generateAllCircuits();
    auto results = data.first;
    auto initialMappingsMap = data.second;
    Result::printHeader();
    for (Result result: results) {
        result.print();
    }

    quMultiEvaluator.setinitMappingsMap(initialMappingsMap);
    quMultiEvaluator.loadFiles();
    quMultiEvaluator.evaluateAllCircuits();

    return 0;
}

//int main(){
//    vector<QuGate*> program;
//    QuGate* gate = nullptr;
//
//    gate = QuGateFactory::getQuGate("h");
//    gate->setArgAtIndex(0, 0);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("cx");
//    gate->setArgAtIndex(0, 0);
//    gate->setArgAtIndex(1, 1);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("cx");
//    gate->setArgAtIndex(0, 0);
//    gate->setArgAtIndex(1, 2);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("cx");
//    gate->setArgAtIndex(0, 1);
//    gate->setArgAtIndex(1, 2);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("x");
//    gate->setArgAtIndex(0, 1);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("z");
//    gate->setArgAtIndex(0, 1);
//    program.push_back(gate);
//
//
//    gate = QuGateFactory::getQuGate("cx");
//    gate->setArgAtIndex(0, 1);
//    gate->setArgAtIndex(1, 2);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("cx");
//    gate->setArgAtIndex(0, 0);
//    gate->setArgAtIndex(1, 2);
//    program.push_back(gate);
//
//    cout << "cancellations: " << QuCircuitOptimizer::performCNOTCancellations(program) << endl;
//    return 0;
//}

/*
unary optimizer cases:
Case: cancellations: 0
h q[0];

Case: cancellations: 0
cx q[0], q[1];

Case: cancellations: 0
h q[0];
cx q[0], q[1];

Case: cancellations: 0
h q[0];
cx q[1], q[0];

Case: cancellations: 0
cx q[0], q[1];
h q[0];

Case: cancellations: 0
cx q[1], q[0];
h q[0];

Case: cancellations: 1
h q[0];
h q[0];
cx q[1], q[0];

Case: cancellations: 0
h q[0];
h q[1];
cx q[1], q[0];

Case: cancellations: 1
h q[1];
h q[1];
cx q[1], q[0];

Case: cancellations: 0
h q[1];
h q[0];
cx q[1], q[0];

Case: cancellations: 0
h q[0];
cx q[1], q[0];
h q[0];

Case: cancellations: 0
h q[0];
cx q[1], q[0];
h q[1];

Case: cancellations: 0
h q[1];
cx q[1], q[0];
h q[1];

Case: cancellations: 1
h q[2];
cx q[1], q[0];
h q[2];

Case: cancellations: 0
h q[1];
cx q[1], q[0];
h q[0];

Case: cancellations: 0
h q[2];
cx q[1], q[0];
h q[3];

Case: cancellations: 0
cx q[1], q[0];
h q[0];
h q[1];
cx q[1], q[0];

Case: cancellations: 1
cx q[1], q[0];
h q[0];
h q[0];
cx q[1], q[0];

Case: cancellations: 0
cx q[1], q[0];
h q[0];
cx q[1], q[0];
h q[0];

Case: cancellations: 1
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];

Case: cancellations: 3
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];

Case: cancellations: 4
h q[2];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[2];

Case: cancellations: 3
h q[2];
cx q[1], q[0];
h q[3];
cx q[1], q[0];
h q[2];
cx q[1], q[0];
h q[3];
cx q[1], q[0];
h q[2];


 */