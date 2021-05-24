//
// Created by SHA on 2/21/20.
//

#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <util/Constants.h>
#include <algorithm>

#include "Config.h"
#include "QuMultiGenerator.h"
#include "QuCircuitGenerator.h"
#include "QuSwapStrategy.h"
#include "QuSmartSwapper.h"
#include "util/Util.h"
#include "util/Result.h"

using namespace std;

QuMultiGenerator::QuMultiGenerator(const string& inputDirectory, const string& outputDirectory, QuArchitecture& quArchitecture)
        : inputDirectory(inputDirectory), outputDirectory(outputDirectory), quArchitecture(quArchitecture){
    loadFiles();
//    remove((directoryPath + outputSubDirectoryName).c_str());
// ubuntu
//    mkdir((outputDirectory).c_str(), 0777);
}

// reads file names and saves in a vector
// also generates the output file names and saves them in another vector
void QuMultiGenerator::loadFiles() {
    DIR* dir = nullptr;
    struct dirent *dirPtr;

    dir = opendir(inputDirectory.c_str());
    while ((dirPtr = readdir(dir)) != nullptr) {
        string temp = string(dirPtr->d_name);
        if(temp.find(Constants::FILE_EXTENSION) != string::npos){
            inputFiles.push_back(temp);
            outputFiles.push_back("output_" + temp);
        }
    }
    closedir(dir);
}


pair<vector<Result>, unordered_map<string, QuMapping>> QuMultiGenerator::generateAllCircuits() {
    unordered_map<string, QuMapping> initialMappingsMap;
    vector<shared_ptr<QuGate>> finalProgram;

    setParams(INITIAL_MAPPING_THRESHOLD, MAPPING_THRESHOLD, RESTRICTED_QUBITS);

    for(unsigned int i=0; i<inputFiles.size(); i++) {
        string file = inputFiles[i].substr(0, inputFiles[i].length() - 5); //removing .qasm extension
        int runs = 1;

        cout << file << endl;
        int gatesOriginal = 0;
        int depthProposed = 0;
        double timeProposed = 0;
        unsigned int gatesProposedOptimized = 0;
        vector<QuMapping> initialMappings;
        Util::println(file + " : ");
        QuCircuitGenerator quCircuitGenerator(quArchitecture.getN(), file);
        QuCircuit &circuit = quCircuitGenerator.getCircuit();
        Util::timeIt(false);
        QuMappingInitializer mappingInitializer(circuit, quArchitecture);
        int x = restrictedQubits;
//        int x = circuit.getN() - MAX_PERMUTATION_N;
//        int x = circuit.getN() - MAX_PERMUTATION_N;
        if(INIT_MAPPING_ALL_PERMUTATIONS)
            initialMappings = mappingInitializer.generateAllPermutationInitialMappings();
        if (INIT_MAPPING_ZERO_COST_PERMUTATIONS)
            initialMappings = mappingInitializer.generateAllZeroCostInitialMappings(x);

        // sha
//        QuMapping::printAll(initialMappings);

        cout << "Initial Mappings generated!" << endl;

        vector<QuMapping> sample;
        if (initialMappings.size() > initialMappingsThreshold) {
            if (RANDOM_SAMPLING_INIT_MAPPINGS) {
                Util::randomSampling(initialMappings, sample, INITIAL_MAPPING_THRESHOLD);
                initialMappings = sample;
            }
            else
                initialMappings.erase(initialMappings.begin() + initialMappingsThreshold, initialMappings.end());
        }


////////////////////////
        pair<int, QuMapping> costNMapping;
        if (INIT_MAPPING_1_BY_1)
            costNMapping = findMinCostUsingInitialMappings1by1(circuit, initialMappings, finalProgram);
        if (INIT_MAPPING_TOGETHER)
            costNMapping = findMinCostUsingInitialMappingsTogether(circuit, initialMappings, finalProgram);

//        auto minCost = costNMapping.first;
        auto initMapping = costNMapping.second;


//////////////////////
        quCircuitGenerator.makeProgramFile(outputDirectory + outputFiles[i], finalProgram);
        initialMappingsMap.insert(make_pair(circuit.getFileName(), initMapping));

    }
    return make_pair(results, initialMappingsMap);
}

pair<vector<Result>, unordered_map<string, QuMapping>> QuMultiGenerator::findCostFor1Benchmark(vector<QuMapping>& initialMappings, string benchmark) {
    unordered_map<string, QuMapping> initialMappingsMap;
    vector<shared_ptr<QuGate>> finalProgram;

//    cout << "file: " << benchmark << endl;
    QuCircuitGenerator quCircuitGenerator(quArchitecture.getN(), benchmark);
    QuCircuit &circuit = quCircuitGenerator.getCircuit();
    Util::timeIt(false);

    pair<int, QuMapping> costNMapping;
    if (INIT_MAPPING_1_BY_1)
        costNMapping = findMinCostUsingInitialMappings1by1(circuit, initialMappings, finalProgram);
    if (INIT_MAPPING_TOGETHER)
        costNMapping = findMinCostUsingInitialMappingsTogether(circuit, initialMappings, finalProgram);

//        auto minCost = costNMapping.first;
    auto initMapping = costNMapping.second;

//////////////////////
    quCircuitGenerator.makeProgramFile(outputDirectory + "output_" + benchmark, finalProgram);
    initialMappingsMap.insert(make_pair(circuit.getFileName(), initMapping));

    return make_pair(results, initialMappingsMap);
}



pair<int, QuMapping> QuMultiGenerator::findMinCostUsingInitialMappings1by1(QuCircuit &circuit, vector<QuMapping> initialMappings, vector<shared_ptr<QuGate>>& minFinalProgram) {
    unsigned int minGates = INT32_MAX;
    unsigned int minSwaps = INT32_MAX;
    unsigned int minHadamards = INT32_MAX;
    unsigned int minGatesProposedOptimized = INT32_MAX;
    unsigned int gatesOriginal = circuit.getInstructions0().size();
    unsigned int gatesProposedOptimized;
    QuMapping minMapping(quArchitecture.getN());

    for (int i=0; i<initialMappings.size(); i++) {
//        cout << "<" << circuit.getFileName() << "> Mapping # " << i << " of " << initialMappings.size() << ": ";
        QuMapping& mapping = initialMappings[i];
        vector<QuMapping> singleInitMapping;
        singleInitMapping.push_back(mapping);
        QuSwapStrategy *strategy = new QuSmartSwapper(circuit, quArchitecture, singleInitMapping);

        auto data = strategy->findTotalSwaps();
        auto totalCost = data.first;
        auto initMapping = data.second;
        unsigned int gatesProposed = totalCost + gatesOriginal;
        unsigned int hadamards = circuit.getHadamards();
        unsigned int swaps = circuit.getSwaps();

        //todo optimizations
        gatesProposedOptimized = gatesProposed - circuit.getOptimizations();
        // todo get layer from finalProgram
//        depthProposed = quCircuitGenerator.getLayer() + 1;
    // todo time elapsed
        if (gatesProposedOptimized < minGatesProposedOptimized) {
            minGatesProposedOptimized = gatesProposedOptimized;
            minGates = gatesProposed;
            minSwaps = swaps;
            minHadamards = hadamards;
            minFinalProgram =  circuit.getInstructions1();
            minMapping = initMapping;
        }

        cout << "g+: " << minGatesProposedOptimized;
        cout << "  Mapping: [" << minMapping.toString() << "]" << endl;

        delete strategy;
    }
    results.push_back(Result(circuit.getFileName(), minSwaps, gatesOriginal, minGates, -1, minHadamards, -1, minGatesProposedOptimized, circuit.getN()));

    return make_pair(minGates, minMapping);
}

pair<int, QuMapping> QuMultiGenerator::findMinCostUsingInitialMappingsTogether(QuCircuit &circuit, vector<QuMapping> initialMappings, vector<shared_ptr<QuGate>>& minFinalProgram) {
    vector<int> costs;
    QuSwapStrategy *strategy = new QuSmartSwapper(circuit, quArchitecture, initialMappings);

    auto data = strategy->findTotalSwaps();
    unsigned int totalCost = data.first;
    auto initMapping = data.second;
    unsigned int hadamards = circuit.getHadamards();
    unsigned int swaps = circuit.getSwaps();
    int gatesOriginal = circuit.getInstructions0().size();
    unsigned int gatesProposed = totalCost + gatesOriginal;

    //todo optimizations
    unsigned int gatesProposedOptimized = gatesProposed - circuit.getOptimizations();
    // todo get layer from finalProgram
//        depthProposed = quCircuitGenerator.getLayer() + 1;
    // todo time elapsed

    delete strategy;
//    results.push_back(Result(circuit.getFileName(), swaps, gatesOriginal, gatesProposed, -1, hadamards, -1, gatesProposedOptimized));
    results.push_back(Result(circuit.getFileName(), swaps, gatesOriginal, gatesProposed, -1, hadamards, -1, gatesProposedOptimized, circuit.getN()));

    return make_pair(gatesProposed, initMapping);
}

void QuMultiGenerator::setParams(int initialMappingsThreshold, int otherMappingsThreshold, int restrictedQubits) {
    this->initialMappingsThreshold = initialMappingsThreshold;
    this->otherMappingsThreshold = otherMappingsThreshold;
    this->restrictedQubits = restrictedQubits;
}


/*
 *
////            vector<int> costs = findCostUsingInitialMappings1by1(circuit, initialMappings);
//        vector<int> costs = findCostUsingInitialMappingsTogether(circuit, initialMappings, minFinalProgram);
//
//        sort(costs.begin(), costs.end());
//        cout << "OPTIMAL COST: " << costs[0] << endl;
//        int gatesProposed = costs[0];

 */