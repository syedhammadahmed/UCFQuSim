//
// Created by hammad on 2/21/20.
//

#ifndef UCFQUSIM_QUMULTIGENERATOR_H
#define UCFQUSIM_QUMULTIGENERATOR_H


#include <core/QuArchitecture.h>
#include <string>
#include <vector>
using namespace std;

class QuMultiGenerator {
private:
    string inputDirectory;
    string outputDirectory;

    QuArchitecture& quArchitecture;  // reference to instance of the physical architecture
    vector<string> inputFiles;
    vector<string> outputFiles;

    void loadFiles();

public:
    QuMultiGenerator(const string inputDirectory, const string outputDirectory, QuArchitecture& quArchitecture);
    void generateAllCircuits();

    void timeIt(bool isEnd);
};


#endif //UCFQUSIM_QUMULTIGENERATOR_H
