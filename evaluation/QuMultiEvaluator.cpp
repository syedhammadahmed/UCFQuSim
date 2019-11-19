//
// Created by hammad on 11/19/19.
//

#include <core/QuCircuit.h>
#include <core/QuCircuitBuilder.h>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include "QuMultiEvaluator.h"
#include "QuCircuitEvaluator.h"

using namespace std;

QuMultiEvaluator::QuMultiEvaluator(const string &directoryPath, QuArchitecture& quArchitecture)
                   : directoryPath(directoryPath), quArchitecture(quArchitecture), outputSubDirectoryName("output/") {
    loadFiles();
//    remove((directoryPath + outputSubDirectoryName).c_str());
    mkdir( (directoryPath + outputSubDirectoryName).c_str(), 0777 );
}

void QuMultiEvaluator::evaluateAll(){
    for(int i=0; i<inputFiles.size(); i++) {
        QuCircuit circuit(quArchitecture.getN());
        QuCircuitBuilder quCircuitBuilder(circuit);
        string file = inputFiles[i];
        cout << "Processing input file: " << file << "..." << endl;
        quCircuitBuilder.buildFromFile(directoryPath + file);
        circuit.initializeMappings(NULL); // NULL is for trivial mapping x[i] = i
        int totalSwaps = circuit.findTotalSwaps(quArchitecture.getCouplingMap());
//        circuit.printInstructions();
        quCircuitBuilder.setInstructions(circuit.getInstructionsV1());
        cout << "Generating intermediate output file: " << outputFiles[i] << ": ";
        cout << "Total Swap Gates inserted: " << totalSwaps << endl;
        quCircuitBuilder.makeProgramFile(directoryPath + outputSubDirectoryName + outputFiles[i]);


        string ofile = outputFiles[i];
        QuCircuit testCircuit(quArchitecture.getN());
        QuCircuitBuilder testQuCircuitBuilder(testCircuit);
        cout << "Evaluating intermediate output file: " << ofile << ": ";
        testQuCircuitBuilder.buildFromFile(directoryPath + outputSubDirectoryName + ofile);
        QuCircuitEvaluator quCircuitEvaluator(testCircuit);
        bool satisfied = quCircuitEvaluator.evaluateCNOTConstraints(quArchitecture.getCouplingMap());
        cout << endl;
    }
}

void QuMultiEvaluator::loadFiles() {
    DIR* dir = NULL;
    struct dirent *dirPtr;

    dir = opendir(directoryPath.c_str());
    while ((dirPtr = readdir(dir)) != NULL) {
        string temp = string(dirPtr->d_name);
        if(temp.find(".qasm") != string::npos){
            inputFiles.push_back(temp);
            outputFiles.push_back("output_" + temp);
        }
    }
    closedir(dir);
}

