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
    string directory;

    QuArchitecture& quArchitecture;  // physical architecture
    vector<string> files;

    void loadFiles();

public:
    QuMultiEvaluator(const string directory, QuArchitecture& quArchitecture);
    void evaluateAllCircuits();

};



#endif //UCFQUSIM_QUMULTIEVALUATOR_H
