//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUMULTIEVALUATOR_H
#define UCFQUSIM_QUMULTIEVALUATOR_H

#include <string>
#include <vector>

#include <core/QuArchitecture.h>

using namespace std;

class QuMultiEvaluator {
private:
    string directoryPath;
    string outputSubDirectoryName;

    QuArchitecture& quArchitecture;  // physical architecture
    vector<string> inputFiles;
    vector<string> outputFiles;

    void loadFiles();

public:
    QuMultiEvaluator(const string &directoryPath, QuArchitecture& quArchitecture);
    void evaluateAll();

};


#endif //UCFQUSIM_QUMULTIEVALUATOR_H
