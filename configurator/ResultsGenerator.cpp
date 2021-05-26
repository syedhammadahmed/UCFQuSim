//
// Created by hammad on 5/23/21.
//

#include "util/Result.h"
#include <vector>
#include "util/Util.h"
#include "util/Constants.h"
#include <fstream>
#include "core/QuMapping.h"
#include "ResultsGenerator.h"


pair<vector<Result>, unordered_map<string, QuMapping>> ResultsGenerator::generateResultsFrom1MappingsFile(string file) {
//    for(unsigned int i=0; i<inputFiles.size(); i++) {
        // for every input mapping file
           // find name of benchmark
           // read file and load mappings
           // find cost and save to a  new output results file
        string benchmark = file.substr(0, file.find_last_of('_')); //removing file batch number

        cout << file << "; (" << benchmark << ")" << endl;

        auto temp = loadInitialMappingsFromFile(benchmark + "/" + file);
        auto result = quMultiGenerator.findCostFor1Benchmark(temp, benchmark);

//    }
    return make_pair(result.first, result.second);
}

vector<QuMapping> ResultsGenerator::loadInitialMappingsFromFile(string file) {
    vector<QuMapping> initialMappings;
    ifstream ifs;
    file = Constants::MAPPINGS_FILES_DIRECTORY_RPATH + file;
    ifs.open(file);
    int mappings;
    ifs >> mappings;
    for (int i = 0; i < mappings; ++i) {
        string line;
        getline(ifs, line);
        getline(ifs, line);
        auto mappingsVals = Util::tokenizeInt(line, " ");
        QuMapping mapping(mappingsVals.size());
        mapping.init(mappingsVals);
        initialMappings.push_back(mapping);
    }
    return initialMappings;
}

ResultsGenerator::ResultsGenerator(QuMultiGenerator& quMultiGenerator): quMultiGenerator(quMultiGenerator) {

}

ResultsGenerator::ResultsGenerator(QuMultiGenerator &quMultiGenerator, string mappingsDirectory): quMultiGenerator(quMultiGenerator) {

}
