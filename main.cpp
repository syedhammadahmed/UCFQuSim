#include <iostream>
#include <iomanip>
#include <string>
#include <generator/Config.h>
#include <util/QuFileConverter.h>
#include <core/gates/QuGateFactory.h>
#include <generator/QuCircuitOptimizer.h>
#include <generator/QuCircuitGenerator.h>
#include <configurator/ResultsGenerator.h>

#include "evaluator/QuMultiEvaluator.h"
#include "generator/QuMultiGenerator.h"
#include "util/Constants.h"
#include "util/Util.h"

using namespace std;
 // for script
/*
int main(int argc, char *argv[]) {
    string inputDirectory = argv[1];
    string outputDirectory = argv[2];
    string mappingsDirectory = argv[3];
    int architectureSize = stoi(argv[4]);
    string mappingsFile = argv[5];

    QuArchitecture architectureQX(architectureSize); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);
    ResultsGenerator resultsGenerator(quMultiGenerator, mappingsDirectory);
    auto data = resultsGenerator.generateResultsFrom1MappingsFile(mappingsFile);

    auto results = data.first;
    auto initialMappingsMap = data.second;

    for (Result result: results)
        result.print();
    return 0;
}*/

// to make mappings file
/*
int main() {
//    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";

    QuArchitecture architectureQX(Constants::QX5_N); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);
    auto data = quMultiGenerator.generateAllCircuits();

    return 0;
}
*/
/*
 * main main
*/
int main() {
//    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";

    if (Util::verbose)
        cout << "Input File Directory: " << inputDirectory << endl;

//    QuArchitecture architectureQX5(physicalQuBitsQX5); // includes the coupling map having CNOT constraints
    QuArchitecture architectureQX(Constants::QX2_N); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);

    auto data = quMultiGenerator.generateAllCircuits();
    auto results = data.first;
    auto initialMappingsMap = data.second;

    cout << endl;
    cout << "Flexibility in SWAPS: " << Constants::FLEXIBILITY_NO_OF_SWAPS << endl;
    cout << ((INIT_MAPPING_ALL_PERMUTATIONS) ? "ALL PERMUTATIONS, " : "ZERO COST PERMUTATIONS, ");
    cout << ((INIT_MAPPING_1_BY_1) ? "1-by-1" : "TOGETHER");
    cout << endl;
    Result::printHeader();
    for (Result result: results) {
        result.print();
//        Util::setVerbose();
        string name = "output_" + result.getFile() + ".qasm";
        QuMapping initialMapping = initialMappingsMap[result.getFile()];
//        initialMapping.print();
//        Util::resetVerbose();
    }

    QuMultiEvaluator quMultiEvaluator(architectureQX, initialMappingsMap);
    quMultiEvaluator.evaluateAllCircuits();

    return 0;
}


/*
//
//int main(int argc, char *argv[]) {
////    Util::verbose = true;
//    cout << "Processing files... this may take a while..." << endl;
//    string inputDirectory = "../input/";
//    string outputDirectory = "../output/";
//
//    if (Util::verbose)
//        cout << "Input File Directory: " << inputDirectory << endl;
//
////    QuArchitecture architectureQX5(physicalQuBitsQX5); // includes the coupling map having CNOT constraints
//    QuArchitecture architectureQX(Constants::QX5_N); // includes the coupling map having CNOT constraints
//    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);
//    ResultsGenerator resultsGenerator(quMultiGenerator);
//
//    vector<string> inputFiles = Util::getFileList(Constants::MAPPINGS_FILES_DIRECTORY_RPATH);
//    auto data = resultsGenerator.generateResultsFrom1MappingsFile(inputFiles[0]);
//
//    auto results = data.first;
//    auto initialMappingsMap = data.second;
//
//    cout << endl;
//    cout << "Flexibility in SWAPS: " << Constants::FLEXIBILITY_NO_OF_SWAPS << endl;
//    cout << ((INIT_MAPPING_ALL_PERMUTATIONS) ? "ALL PERMUTATIONS, " : "ZERO COST PERMUTATIONS, ");
//    cout << ((INIT_MAPPING_1_BY_1) ? "1-by-1" : "TOGETHER");
//    cout << endl;
//    Result::printHeader();
//    for (Result result: results) {
//        result.print();
////        Util::setVerbose();
//        string name = "output_" + result.getFile() + ".qasm";
//        QuMapping initialMapping = initialMappingsMap[result.getFile()];
////        initialMapping.print();
////        Util::resetVerbose();
//    }
//
//    QuMultiEvaluator quMultiEvaluator(architectureQX, initialMappingsMap);
//    quMultiEvaluator.evaluateAllCircuits();
//
//    return 0;
//}

//int main() {
//
//    string inputDirectory = "../input/";
//    string outputDirectory = "../output/";
//    if (Util::verbose)
//        cout << "Input File Directory: " << inputDirectory << endl;
//
////    QuArchitecture architectureQX5(physicalQuBitsQX5); // includes the coupling map having CNOT constraints
//    QuArchitecture architectureQX(Constants::QX5_N); // includes the coupling map having CNOT constraints
//    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX);
//
//    auto data = quMultiGenerator.generateAllCircuits();
//
//    return 0;
//}


//int main() {
//    QuFileConverter quFileConverter(Constants::QCODE_INPUT_FILES_DIRECTORY_RPATH, Constants::QCODE_OUTPUT_FILES_DIRECTORY_RPATH);
//    quFileConverter.convertAllFiles();
//    return 0;
//}


gate = QuGateFactory::getQuGate("x");
gate->setArgAtIndex(0, 3);
program.push_back(gate);

gate = QuGateFactory::getQuGate("cx");
gate->setArgAtIndex(0, 3);
gate->setArgAtIndex(1, 4);
program.push_back(gate);
int main(){
    vector<shared_ptr<QuGate>> program;
    shared_ptr<QuGate> gate = nullptr;


    gate = QuGateFactory::getQuGate("h");
    gate->setArgAtIndex(0, 0);
    program.push_back(gate);

    gate = QuGateFactory::getQuGate("cx");
    gate->setArgAtIndex(0, 0);
    gate->setArgAtIndex(1, 1);
    program.push_back(gate);

    gate = QuGateFactory::getQuGate("cx");
    gate->setArgAtIndex(0, 0);
    gate->setArgAtIndex(1, 2);
    program.push_back(gate);

    gate = QuGateFactory::getQuGate("cx");
    gate->setArgAtIndex(0, 1);
    gate->setArgAtIndex(1, 2);
    program.push_back(gate);

    gate = QuGateFactory::getQuGate("cx");
    gate->setArgAtIndex(0, 1);
    gate->setArgAtIndex(1, 2);
    program.push_back(gate);

    gate = QuGateFactory::getQuGate("cx");
    gate->setArgAtIndex(0, 0);
    gate->setArgAtIndex(1, 2);
    program.push_back(gate);

    int uc = QuCircuitOptimizer::performUnaryCancellations(Constants::QX4_N, program);
    int cc = QuCircuitOptimizer::performCNOTCancellations(Constants::QX4_N, program);
    cout << "unary cancellations: "  << uc << endl;
    cout << "cnot cancellations: "  << cc << endl;

    return 0;
}
*/

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