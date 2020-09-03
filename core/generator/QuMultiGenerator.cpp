//
// Created by hammad on 2/21/20.
//

#include <util/Util.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <util/Result.h>
#include "QuMultiGenerator.h"
#include "QuCircuitGenerator.h"

using namespace std;

QuMultiGenerator::QuMultiGenerator(const string& inputDirectory, const string& outputDirectory, QuArchitecture& quArchitecture)
        : inputDirectory(inputDirectory), outputDirectory(outputDirectory), quArchitecture(quArchitecture){
    loadFiles();
//    remove((directoryPath + outputSubDirectoryName).c_str());
// ubuntu
//    mkdir((outputDirectory).c_str(), 0777);

// windows
//
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


vector<Result> QuMultiGenerator::generateAllCircuits() {
    vector<Result> results;
    for(unsigned int i=0; i<inputFiles.size(); i++) {
        string inputFileAbsPath = inputDirectory + inputFiles[i];
        string file = inputFiles[i].substr(0, inputFiles[i].length() - 5); //removing .qasm extension
        // reads the qasm file and makes a default circuit
        QuCircuitGenerator quCircuitGenerator(quArchitecture, inputFileAbsPath);
//        quArchitecture.printCouplingMatrix();
//        cout << quArchitecture;
        QuCircuit& circuit = quCircuitGenerator.getCircuit();
        circuit.setFileName(inputFiles[i]);
        Util::println(file + " : ");
        Util::timeIt(false);
        int gatesOriginal = circuit.getInstructions().size();
        int totalSwaps = circuit.findTotalSwaps(quArchitecture);
        double timeProposed = Util::timeIt(true); // todo loss due to cast

        int swaps = circuit.getSwaps();
        int hadamards = circuit.getHadamards();
        unsigned int gatesProposed = circuit.getInstructionsV1().size() + totalSwaps * 6; // 7 elementary gates per swap, 1 already counted as swap itself

        quCircuitGenerator.setInstructions(circuit.getInstructionsV1());
//        quCircuitGenerator.buildGrid();
        quCircuitGenerator.makeProgramFile(outputDirectory + outputFiles[i]);
        int depthProposed = quCircuitGenerator.getLayer() + 1;
        results.push_back(Result(file, swaps, gatesOriginal, gatesProposed, depthProposed, hadamards, timeProposed));
    }
    return results;
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
