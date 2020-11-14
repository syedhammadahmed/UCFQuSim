#include <iostream>
#include <iomanip>
#include <string>

#include "evaluator/QuMultiEvaluator.h"
#include "core/generator/QuMultiGenerator.h"
#include "util/Result.h"
#include "util/Util.h"
#include "AllShortestPathsFinder.h"

using namespace std;

int main() {
    const int quBits = 16;

    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";
    if (Util::verbose)
        cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX5);

    auto data = quMultiGenerator.generateAllCircuits();
    auto results = data.first;
    auto initialMappingsMap = data.second;
    Result::printHeader();
    for (Result result: results) {
        result.print();
    }

    quMultiEvaluator.setinitMappingsMap(initialMappingsMap);
    quMultiEvaluator.loadFiles();
    quMultiEvaluator.evaluateAllCircuits();

    return 0;
}

//#include <algorithm>
//#include <iostream>
//#include <iterator>
//#include <random>
//#include <vector>
//#include "util/Util.h"
//
//using namespace std;
//
//
//int main() {
//    vector<int> temp = {1,2,3,4,5,6,7,8,9,10};
//    vector<int> temp2;
//     size = 5;
//    Util::sampler(temp, temp2, size);
//    for (int i: temp2)
//        std::cout << i << " "; //prints 5 randomly chosen values from population vector
//}
//
//#include <iostream>
//#include <random>
//#include <vector>
//#include <algorithm>
//#include "util/Util.h"
//#include "core/QuMapping.h"
//
//int main() {
//    string temp1 = "0";
//    QuMapping temp(temp1);
//    vector<QuMapping> population;
//    vector<QuMapping> sample;
//    population.push_back(temp);
//    population.push_back(temp);
//    population.push_back(temp);
//    population.push_back(temp);
//    int sampleSize = 3;
//    Util::randomSampling(population, sample, sampleSize);
//    for(int i = 0; i < sample.size(); i++) std::cout << sample[i].getMappingId() << std::endl;
//}
