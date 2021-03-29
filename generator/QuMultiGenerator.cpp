//
// Created by SHA on 2/21/20.
//

#include <util/Util.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "util/Result.h"
#include "core/Config.h"
#include "QuMultiGenerator.h"
#include "QuCircuitGenerator.h"

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
        if(temp.find(Util::FILE_EXTENSION) != string::npos){
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
        string inputFileAbsPath = inputDirectory + inputFiles[i];
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
            cout << "run #: " << j << endl;
            QuCircuitGenerator quCircuitGenerator(quArchitecture, inputFileAbsPath);
            QuCircuit &circuit = quCircuitGenerator.getCircuit();
            circuit.setFileName(inputFiles[i]);
            Util::println(file + " : ");
            Util::timeIt(false);
            gatesOriginal = circuit.getInstructions0().size();
            auto data = circuit.findTotalSwaps(quArchitecture);
            unsigned int totalCost = data.first;
            auto initMapping = data.second;
            timeProposed = Util::timeIt(true); // todo loss due to cast
            unsigned int hadamards = circuit.getHadamards();
            unsigned int swaps = circuit.getSwaps();
//            int totalCost = hadamards + swaps * 7;
            unsigned int gatesProposed = totalCost + gatesOriginal;
            gatesProposedOptimized = gatesProposed - circuit.getOptimizations();

            quCircuitGenerator.setInstructions(circuit.getInstructions1());
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


//
//void QuMultiGenerator::timeIt(bool isEnd) {
//    time_t start;
//    time_t end;
//    if(!isEnd) {
//        time(&start);
//        // unsync the I/O of C and C++.
//        ios_base::sync_with_stdio(false);
//    }
//    else {
//        time(&end);
//        double time_taken = double(end - start);
//        cout << fixed << time_taken << setprecision(5) << endl;
//    }
//
//}
