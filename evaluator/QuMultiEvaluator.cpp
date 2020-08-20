//
// Created by hammad on 11/19/19.
//

#include <iostream>
#include <dirent.h>
#include "QuMultiEvaluator.h"
#include "QuCircuitEvaluator.h"
#include "generator/QuCircuitGenerator.h"

using namespace std;

QuMultiEvaluator::QuMultiEvaluator(const string directory, QuArchitecture& quArchitecture)
                   : directory(directory), quArchitecture(quArchitecture){
//    loadFiles();
    deletePreviousFiles();

}

void QuMultiEvaluator::evaluateAllCircuits(){

    for(int i=0; i<files.size(); i++) {
        string ofile = files[i]; // todo commented for results printing
        if(files[i] == "." || files[i] == "..")
            continue;
        QuCircuitGenerator testQuCircuitBuilder(quArchitecture);
        cout << "Evaluating intermediate output file: " << ofile << ": ";
        testQuCircuitBuilder.buildFromFile(directory + ofile);
        QuCircuit& testCircuit = testQuCircuitBuilder.getCircuit();
        QuCircuitEvaluator quCircuitEvaluator(testCircuit);
        bool satisfied = quCircuitEvaluator.evaluateCNOTConstraints(quArchitecture.getCouplingMap());
        cout << endl;
    }
}

void QuMultiEvaluator::loadFiles() {
    DIR* dir = NULL;
    struct dirent *dirPtr;

    dir = opendir(directory.c_str());
    while ((dirPtr = readdir(dir)) != NULL) {
        string temp = string(dirPtr->d_name);
            files.push_back(temp);
    }
    closedir(dir);
}

void QuMultiEvaluator::deletePreviousFiles() {
    DIR* dir = NULL;
    struct dirent *dirPtr;
    string filepath;

    dir = opendir(directory.c_str());
    while ((dirPtr = readdir(dir)) != NULL) {
        filepath =  directory + "/" + dirPtr->d_name;
        remove(filepath.c_str());
    }
    closedir(dir);
}