//
// Created by hammad on 2/21/20.
//

#include <util/Util.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include "QuMultiGenerator.h"
#include "QuCircuitGenerator.h"

using namespace std;

QuMultiGenerator::QuMultiGenerator(const string inputDirectory, const string outputDirectory, QuArchitecture& quArchitecture)
        : inputDirectory(inputDirectory), outputDirectory(outputDirectory), quArchitecture(quArchitecture){
    loadFiles();
//    remove((directoryPath + outputSubDirectoryName).c_str());
    mkdir((outputDirectory).c_str(), 0777);
}

// reads file names and saves in a vector
// also generates the output file names and saves them in another vector
void QuMultiGenerator::loadFiles() {
    DIR* dir = NULL;
    struct dirent *dirPtr;

    dir = opendir(inputDirectory.c_str());
    while ((dirPtr = readdir(dir)) != NULL) {
        string temp = string(dirPtr->d_name);
        if(temp.find(Util::FILE_EXTENSION) != string::npos){
            inputFiles.push_back(temp);
            outputFiles.push_back("output_" + temp);
        }
    }
    closedir(dir);
}


void QuMultiGenerator::generateAllCircuits() {
    cout << "File\t";
    cout << "g\t";
    cout << "s\t";
    cout << "g*\t";
    cout << "d*\t";
    cout << "t*\t";
    cout << endl;

    for(int i=0; i<inputFiles.size(); i++) {
        Util::timeIt(false);
        string inputFileAbsPath = inputDirectory + inputFiles[i];

        // reads the qasm file and makes a default circuit
        QuCircuitGenerator quCircuitGenerator(quArchitecture, inputFileAbsPath);
        QuCircuit& circuit = quCircuitGenerator.getCircuit();
//        QuCircuit* pCircuit = quCircuitGenerator.getCircuit();
//        QuCircuit& circuit = *pCircuit;
        int totalSwaps = circuit.findTotalSwaps(quArchitecture);
//        circuit.printInstructions();

        quCircuitGenerator.setInstructions(circuit.getInstructionsV1());
//        cout << "Input file: " << inputFiles[i] << "; ";
//        cout << "Output file: " << outputFiles[i] << "; ";
        cout << inputFiles[i].substr(0, inputFiles[i].length() - 5) << "\t";
        cout << circuit.getInstructions().size() << "\t";
        cout << totalSwaps << "\t";
        cout << circuit.getInstructions().size() + totalSwaps * 7 << "\t";// 7 elementary gates per swap
        quCircuitGenerator.makeProgramFile(outputDirectory + outputFiles[i]);
        cout << quCircuitGenerator.getLayer() << "\t";
        cout << Util::timeIt(true) << endl;
        //          cout << "Input file: " << inputFiles[i] << "; ";
////        cout << "Output file: " << outputFiles[i] << "; ";
//        cout << "g: " << circuit.getInstructions().size() << "; "; // 7 elementary gates per swap
//        cout << "s: " << totalSwaps << "; ";
//        cout << "g*: " << circuit.getInstructions().size() + totalSwaps * 7 << "; "; // 7 elementary gates per swap
//        quCircuitGenerator.makeProgramFile(directoryPath + outputSubDirectoryName + outputFiles[i]);
//        cout << "d*: " << quCircuitGenerator.getLayer() << "; ";
//        delete pCircuit;
    }
}

void QuMultiGenerator::timeIt(bool isEnd) {
    time_t start;
    time_t end;
    if(!isEnd) {
        time(&start);
        // unsync the I/O of C and C++.
        ios_base::sync_with_stdio(false);
    }
    else {
        time(&end);
        double time_taken = double(end - start);
        cout << fixed << time_taken << setprecision(5) << endl;
    }

}
