#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <evaluator/QuMultiEvaluator.h>
#include <core/generator/QuMultiGenerator.h>
#include <iomanip>
#include <util/Result.h>
#include <util/Util.h>
#include <core/generator/QuMappingInitializer.h>
#include <climits>
#include <algorithm>

#include "core/gates/CNot.h"
#include "core/gates/QuGate.h"
#include "core/QuSimulator.h"
#include "core/gates/Hadamard.h"
#include "core/gates/Not.h"
#include "ShortestPathFinder.h"
#include "core/generator/QuCircuitGenerator.h"
#include "AllShortestPathsFinder.h"

using namespace std;

//int main(){
//    const int quBits = 16;
//    const int MAX_DEPTH = 10;
//    string inputDirectory = "../input/";
//    string outputDirectory = "../output/";
//
//    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
////    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
////    vector<Result> results = quMultiGenerator.generateAllCircuits();
//
//    string inputFileAbsPath = inputDirectory + "sample.qasm";
//    QuCircuitGenerator quCircuitGenerator(architectureQX5, inputFileAbsPath);
//    QuCircuit& circuit = quCircuitGenerator.getCircuit();
//    auto inst = circuit.getInstructions();
//    vector<int> qubits;
//    for (auto i: inst) {
//        qubits.push_back(i->getArgAtIndex(0));
//        qubits.push_back(i->getArgAtIndex(1));
//    }
//    std::sort(qubits.begin(), qubits.end());
//    auto qit = std::unique(qubits.begin(), qubits.begin() + qubits.size());
//    qubits.resize(std::distance(qubits.begin(),qit));
//    for (auto &y: qubits) {
//        cout << y << " ";
//    }
//    cout << endl;
//
//    vector<vector<int>> temp;
//    temp.resize(8);
//    for (auto i: inst) {
//        int s = i->getArgAtIndex(0);
//        int t = i->getArgAtIndex(1);
//        bool done = false;
//        for (auto& x: temp) {
//            for (auto& y: x) {
//                if (s == y) {
//                    x.push_back(t);
//                    done = true;
//                    break;
//                }
//                if (t == y) {
//                    x.push_back(s);
//                    done = true;
//                    break;
//                }
//            }
//        }
//        if (!done) {
//            temp[0].push_back(s);
//            temp[0].push_back(t);
//        }
//    }
//
//    cout << "list: " << endl;
//    for (auto& x: temp) {
//        std::sort(x.begin(), x.end());
//        auto qit = std::unique(x.begin(), x.begin() + x.size());
//        x.resize(std::distance(x.begin(),qit));
//        for (auto &y: x) {
//            cout << y << " ";
//        }
//        cout << endl;
//    }
//
//}


int main() {
    const int quBits = 16;
    const int MAX_DEPTH = 10;
//    Util::mappingInitializer.se = false;
//    Util::verbose = true;
    cout << "Processing files... this may take a while..." << endl;
    string inputDirectory = "../input/";
    string outputDirectory = "../output/";
    if(Util::verbose)
        cout << "Input File Directory: " << inputDirectory << endl;

    QuArchitecture architectureQX5(quBits); // includes the coupling map having CNOT constraints
    QuMultiGenerator quMultiGenerator(inputDirectory, outputDirectory, architectureQX5);
    vector<Result> results = quMultiGenerator.generateAllCircuits();
    Result::printHeader();
    for(Result result: results){
        result.print();
    }

    QuMultiEvaluator quMultiEvaluator(outputDirectory, architectureQX5);
    quMultiEvaluator.loadFiles();
    quMultiEvaluator.evaluateAllCircuits();

    return 0;
}

 /*
int main() {
    QuArchitecture architectureQX5(16); // includes the coupling map having CNOT constraints
    architectureQX5.printCouplingMatrix();
    AllShortestPathsFinder allSPF(architectureQX5.getCouplingMap(), architectureQX5.getN());

    for(int k=1; k<=8; k++) {
        for (int i = 0; i < architectureQX5.getN(); i++) {
            for (int j = 0; j < architectureQX5.getN(); j++) {
                if (i != j) {
                    vector<vector<int>> paths = allSPF.findSingleSourceAllShortestPaths(i, j, k);
                    cout << "(" << i << "," << j << "): [length = " << k << "]" << endl;
                    for (vector<int> path: paths) {
                        Util::printPath(path);
                    }
                    cout << endl;
                }
            }
        }
    }
//    int i = 0, j = 3, k = 1;
//    vector<vector<int>> paths = allSPF.findSingleSourceAllShortestPaths(i, j, k);
//    cout << "(" << i << "," << j << "): [length = " << k << "]" << endl;
//    for (vector<int> path: paths) {
//        Util::printPath(path);
//    }
//    cout << endl;
}
 */

// int main()
// {
////     string str = "012";
//     string str = "0123456789";
//     int n = str.size();
//     vector<string> perms;
//     Util::permute(str, 0, n-1, perms);
//     cout << perms.size();
////     for (string perm: perms) {
////         for(int i=0; i<perm.length(); i++){
////            int val = perm[i] - 48;
////            cout << val << " ";
////         }
////         cout << endl;
////     }
//     return 0;
// }
/*
int main(){
    vector<vector<int>> perms;
//    vector<int> a = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    vector<int> a = {0,1,2,3,4,5,6,7,8};
    for(int i=9; i<16; i++){
        a.push_back(i);
        Util::permute(a, 0, a.size()-1, perms);
        Util::println("total perms: " + to_string(perms.size()));
    }
//    Util::permute(a, 0, a.size()-1, perms);
//    for(vector<int> v: perms) {
//        for (int &i: v)
//            Util::print(to_string(i) + ", ");
//        Util::println("");
//    }
    return 0;
}
 */

/*
void print(std::vector<std::vector<double>> dist, std::vector<std::vector<int>> next) {
    std::cout << "(pair, dist, path)" << std::endl;
    const auto size = next.size();
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            if (i != j) {
                auto u = i + 1;
                auto v = j + 1;
                std::cout << "(" << u << " -> " << v << ", " << dist[i][j]
                          << ", ";
                std::stringstream path;
                path << u;
                do {
                    u = next[u - 1][v - 1];
                    path << " -> " << u;
                } while (u != v);
                std::cout << path.str() << ")" << std::endl;
            }
        }
    }
}

void solve(std::vector<std::vector<int>> w_s, const int num_vertices) {
    std::vector<std::vector<double>> dist(num_vertices);
    for (auto& dim : dist) {
        for (auto i = 0; i < num_vertices; ++i) {
            dim.push_back(INT_MAX);
        }
    }
    for (auto& w : w_s) {
        dist[w[0] - 1][w[1] - 1] = w[2];
    }
    std::vector<std::vector<int>> next(num_vertices);
    for (auto i = 0; i < num_vertices; ++i) {
        for (auto j = 0; j < num_vertices; ++j) {
            next[i].push_back(0);
        }
        for (auto j = 0; j < num_vertices; ++j) {
            if (i != j) {
                next[i][j] = j + 1;
            }
        }
    }
    for (auto k = 0; k < num_vertices; ++k) {
        for (auto i = 0; i < num_vertices; ++i) {
            for (auto j = 0; j < num_vertices; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    print(dist, next);
}

int main() {
    std::vector<std::vector<int>> w = {
            { 1, 3, -2 },
            { 2, 1, 4 },
            { 2, 3, 3 },
            { 3, 4, 2 },
            { 4, 2, -1 },
    };
    int num_vertices = 4;
    solve(w, num_vertices);
    std::cin.ignore();
    std::cin.get();
    return 0;
}
*/