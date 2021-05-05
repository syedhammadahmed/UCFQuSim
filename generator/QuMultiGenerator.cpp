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
        if(INIT_MAPPING_ALL_PERMUTATIONS)
            initialMappings = mappingInitializer.generateAllPermutationInitialMappings();
        if (INIT_MAPPING_ZERO_COST_PERMUTATIONS)
            initialMappings = mappingInitializer.generateAllZeroCostInitialMappings(1);

        // sha
//        QuMapping::printAll(initialMappings);

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

pair<int, QuMapping> QuMultiGenerator::findMinCostUsingInitialMappings1by1(QuCircuit &circuit, vector<QuMapping> initialMappings, vector<shared_ptr<QuGate>>& minFinalProgram) {
    unsigned int minGates = INT32_MAX;
    unsigned int minSwaps = INT32_MAX;
    unsigned int minHadamards = INT32_MAX;
    unsigned int minGatesProposedOptimized = INT32_MAX;
    unsigned int gatesOriginal = circuit.getInstructions0().size();
    unsigned int gatesProposedOptimized;
    QuMapping minMapping(initialMappings[0].getN());

    for (auto& mapping: initialMappings) {
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
        cout << "minGatesProposedOptimized: " << minGatesProposedOptimized << endl;
        cout << "gatesProposedOptimized: " << gatesProposedOptimized << endl;
        cout << "MinMapping: " << minMapping.toString() << endl;
        if (gatesProposedOptimized < minGatesProposedOptimized) {
            minGatesProposedOptimized = gatesProposedOptimized;
            minGates = gatesProposed;
            minSwaps = swaps;
            minHadamards = hadamards;
            minFinalProgram =  circuit.getInstructions1();
            minMapping = initMapping;
        }

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


/*
 *
////            vector<int> costs = findCostUsingInitialMappings1by1(circuit, initialMappings);
//        vector<int> costs = findCostUsingInitialMappingsTogether(circuit, initialMappings, minFinalProgram);
//
//        sort(costs.begin(), costs.end());
//        cout << "OPTIMAL COST: " << costs[0] << endl;
//        int gatesProposed = costs[0];

 */