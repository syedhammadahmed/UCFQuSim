#include <iostream>
#include <iomanip>
#include <string>

#include "evaluator/QuMultiEvaluator.h"
#include "generator/QuMultiGenerator.h"
#include "util/Constants.h"
#include "util/Util.h"

using namespace std;

int main() {
    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";
    if (Util::verbose)
        cout << "Input File Directory: " << inputDirectory << endl;

//    QuArchitecture architectureQX5(physicalQuBitsQX5); // includes the coupling map having CNOT constraints
    QuArchitecture architectureQX(Constants::QX4_N); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);

    auto data = quMultiGenerator.generateAllCircuits();
    auto results = data.first;
    auto initialMappingsMap = data.second;

    Result::printHeader();
    for (Result result: results) {
        result.print();
        Util::setVerbose();
        string name = "output_" + result.getFile() + ".qasm";
        QuMapping initialMapping = initialMappingsMap[name];
//        initialMapping.print();
        Util::resetVerbose();
    }

    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX, initialMappingsMap);
    quMultiEvaluator.evaluateAllCircuits();

    return 0;
}

//int main(){
//    vector<shared_ptr<QuGate>> program;
//    shared_ptr<QuGate> gate = nullptr;
//
//    gate = QuGateFactory::getQuGate("h");
//    gate->setArgAtIndex(0, 0);
//    program.push_back(gate);
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
//    gate = QuGateFactory::getQuGate("h");
//    gate->setArgAtIndex(0, 0);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("h");
//    gate->setArgAtIndex(0, 0);
//    program.push_back(gate);
//
//    gate = QuGateFactory::getQuGate("h");
//    gate->setArgAtIndex(0, 0);
//    program.push_back(gate);
//
//    QuCircuitOptimizer::performUnaryCancellations(program);
//
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