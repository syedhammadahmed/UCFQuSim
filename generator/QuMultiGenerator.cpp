//
// Created by SHA on 2/21/20.
//

#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <util/Constants.h>

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
    vector<Result> results;
    unordered_map<string, QuMapping> initialMappingsMap;

    for(unsigned int i=0; i<inputFiles.size(); i++) {
//        string inputFileAbsPath = inputDirectory + inputFiles[i];
        string file = inputFiles[i].substr(0, inputFiles[i].length() - 5); //removing .qasm extension
        // reads the qasm file and makes a default circuit
        int runs = 1;
        unsigned int minGates = INT32_MAX;
        unsigned int minSwaps = INT32_MAX;
        unsigned int minHadamards = INT32_MAX;
        unsigned int minGatesProposedOptimized = INT32_MAX;

        unsigned int max = 0;
        unsigned int total = 0;
        unsigned int average = 0;
        if (RANDOM_SAMPLING_INIT_MAPPINGS || RANDOM_SAMPLING_MAPPINGS_PRUNING)
            runs = RANDOM_SAMPLING_RUNS;
        cout << file << endl;
        int gatesOriginal = 0;
        int depthProposed = 0;
        double timeProposed = 0;
        unsigned int gatesProposedOptimized = 0;
        for (int j = 0; j < runs; ++j) {
            Util::println(file + " : ");
            cout << "run #: " << j << endl;
            QuCircuitGenerator quCircuitGenerator(quArchitecture.getN(), file);
            QuCircuit &circuit = quCircuitGenerator.getCircuit();
            Util::timeIt(false);
            gatesOriginal = circuit.getInstructions0().size();

            QuSwapStrategy *strategy = new QuSmartSwapper(circuit, quArchitecture);

            auto data = strategy->findTotalSwaps();
            unsigned int totalCost = data.first;
            auto initMapping = data.second;
            timeProposed = Util::timeIt(true); // todo loss due to cast
            unsigned int hadamards = circuit.getHadamards();
            unsigned int swaps = circuit.getSwaps();
//            int totalCost = hadamards + swaps * 7;
            unsigned int gatesProposed = totalCost + gatesOriginal;
            gatesProposedOptimized = gatesProposed - circuit.getOptimizations();

            quCircuitGenerator.makeProgramFile(outputDirectory + outputFiles[i]);
            initialMappingsMap.insert(make_pair(outputFiles[i], initMapping));
            depthProposed = quCircuitGenerator.getLayer() + 1;
            if (gatesProposed < minGates) {
                minGates = gatesProposed;
                minSwaps = swaps;
                minHadamards = hadamards;
                minGatesProposedOptimized = gatesProposedOptimized;
            }
            if (gatesProposed > max)
                max = gatesProposed;
            total += gatesProposed;
            cout <<  "\tg* = " << gatesProposed << endl;
//            results.push_back(Result(file, swaps, gatesOriginal, gatesProposed, depthProposed, hadamards, timeProposed));
        }
        average = total / runs;
        cout << "min : " << minGates << endl;
        cout << "max : " << max << endl;
        cout << "avg : " << average << endl;
        results.push_back(Result(file, minSwaps, gatesOriginal, minGates, depthProposed, minHadamards, timeProposed, gatesProposedOptimized));
//        circuit.printGrid();
//        circuit.printSimpleGrid();
    }
    return make_pair(results, initialMappingsMap);
}