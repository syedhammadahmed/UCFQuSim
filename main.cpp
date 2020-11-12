#include <iostream>
#include <iomanip>
#include <string>

#include "evaluator/QuMultiEvaluator.h"
#include "core/generator/QuMultiGenerator.h"
#include "util/Result.h"
#include "util/Util.h"
#include "AllShortestPathsFinder.h"

using namespace std;

int main() {
    const int quBits = 16;

    Util::verbose = true;
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