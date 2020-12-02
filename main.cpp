#include <iostream>
#include <iomanip>
#include <string>

#include "evaluator/QuMultiEvaluator.h"
#include "core/generator/QuMultiGenerator.h"
#include "util/Result.h"
#include "util/Util.h"
#include "AllShortestPathsFinder.h"
#include "core/generator/QuSmartSwapper.h"
#include "core/gates/QuGateFactory.h"
using namespace std;
//
//int main() {
//    const int quBits = 16;
//
////    Util::verbose = true;
//    cout << "Processing files... this may take a while..." << endl;
//    string inputDirectory = "../input/";
//    string outputDirectory = "../output/";
//    if (Util::verbose)
//        cout << "Input File Directory: " << inputDirectory << endl;
//
//    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
//    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
//    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX5);
//
//    auto data = quMultiGenerator.generateAllCircuits();
//    auto results = data.first;
//    auto initialMappingsMap = data.second;
//    Result::printHeader();
//    for (Result result: results) {
//        result.print();
//    }
//
//    quMultiEvaluator.setinitMappingsMap(initialMappingsMap);
//    quMultiEvaluator.loadFiles();
//    quMultiEvaluator.evaluateAllCircuits();
//
//    return 0;
//}

int main(){
    vector<QuGate*> program;
    QuGate* gate = nullptr;

    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 3);
    program.push_back(gate);
    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 3);
    program.push_back(gate);
    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 3);
    program.push_back(gate);
    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 3);
    program.push_back(gate);
    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 3);
    program.push_back(gate);
    QuSmartSwapper::performUnaryCancellations(program);
    return 0;
}

/*
unary optimizer cases:
Case:
h q[0];

Case:
cx q[0], q[1];

Case:
h q[0];
cx q[0], q[1];

Case:
h q[0];
cx q[1], q[0];

Case:
cx q[0], q[1];
h q[0];

Case:
cx q[1], q[0];
h q[0];

Case:
h q[0];
h q[0];
cx q[1], q[0];

Case:
h q[0];
h q[1];
cx q[1], q[0];

Case:
h q[1];
h q[1];
cx q[1], q[0];

Case:
h q[1];
h q[0];
cx q[1], q[0];

Case:
h q[0];
cx q[1], q[0];
h q[0];

Case:
h q[0];
cx q[1], q[0];
h q[1];

Case:
h q[1];
cx q[1], q[0];
h q[1];

Case:
h q[1];
cx q[1], q[0];
h q[0];

Case:
h q[0];
h q[0];
cx q[1], q[0];

Case:
h q[0];
h q[1];
cx q[1], q[0];

Case:
h q[1];
h q[1];
cx q[1], q[0];

Case:
h q[1];
h q[0];
cx q[1], q[0];

 */