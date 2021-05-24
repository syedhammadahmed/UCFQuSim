//
// Created by hammad on 5/23/21.
//

#ifndef UCFQUSIM_RESULTSGENERATOR_H
#define UCFQUSIM_RESULTSGENERATOR_H


#include "generator/QuMultiGenerator.h"

class ResultsGenerator {
private:
    QuMultiGenerator& quMultiGenerator;

public:
    ResultsGenerator(QuMultiGenerator& quMultiGenerator);
    ResultsGenerator(QuMultiGenerator& quMultiGenerator, string mappingsDirectory);

    vector<QuMapping> loadInitialMappingsFromFile(string file);
    pair<vector<Result>, unordered_map<string, QuMapping>> generateResultsFrom1MappingsFile();

    pair<vector<Result>, unordered_map<string, QuMapping>> generateResultsFrom1MappingsFile(string file);
};


#endif //UCFQUSIM_RESULTSGENERATOR_H
