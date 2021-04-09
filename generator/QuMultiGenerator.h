//
// Created by SHA on 2/21/20.
//

#ifndef UCFQUSIM_QUMULTIGENERATOR_H
#define UCFQUSIM_QUMULTIGENERATOR_H


#include <core/QuArchitecture.h>
#include <string>
#include <vector>
#include <core/QuMapping.h>
#include <unordered_map>
#include <core/QuCircuit.h>
#include "util/Result.h"

using namespace std;

class QuMultiGenerator {
private:
    string inputDirectory;
    string outputDirectory;

    QuArchitecture& quArchitecture;  // reference to instance of the physical architecture
    vector<string> inputFiles;
    vector<string> outputFiles;

    vector<Result> results;
    void loadFiles();

public:
    QuMultiGenerator(const string& inputDirectory, const string& outputDirectory, QuArchitecture& quArchitecture);
    pair<vector<Result>, unordered_map<string, QuMapping>> generateAllCircuits();

//    void timeIt(bool isEnd);
//    vector<int> findMinCostUsingInitialMappings1by1(QuCircuit &circuit, vector<QuMapping> initialMappings);
//    vector<int> findMinCostUsingInitialMappingsTogether(QuCircuit &circuit, vector<QuMapping> initialMappings);
    pair<int, QuMapping> findMinCostUsingInitialMappings1by1(QuCircuit &circuit, vector<QuMapping> initialMappings, vector<shared_ptr<QuGate>>& minFinalProgram);
    pair<int, QuMapping> findMinCostUsingInitialMappingsTogether(QuCircuit &circuit, vector<QuMapping> initialMappings, vector<shared_ptr<QuGate>>& minFinalProgram);

};


#endif //UCFQUSIM_QUMULTIGENERATOR_H
