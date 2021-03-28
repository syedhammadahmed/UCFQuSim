//
// Created by SHA on 9/26/19.
//

#ifndef UCFQUSIM_QUCIRCUIT_H
#define UCFQUSIM_QUCIRCUIT_H

#include <vector>
#include <ostream>
#include <string>

#include "gates/QuGate.h"
#include "QuMapping.h"
#include "QuArchitecture.h"

using namespace std;

class QuCircuit {

private:
    string fileName; // circuit input file name (absolute path)
    int rows; // # of physical quBits (max rows)
    int cols; // depth
    int n; // # of logical qubits

    shared_ptr<QuGate>** grid;
    int** simpleGrid;

    vector<shared_ptr<QuGate>> instructions0; // original program
    vector<shared_ptr<QuGate>> instructions1; // modified program after satisfying constraints

    int hadamards;
    int swaps;
    int cost;
    int optimizations;

    vector<int> srcFrequencies;
    vector<int> destFrequencies;

public:
    QuCircuit();
    explicit QuCircuit(int rows);
    QuCircuit(int rows, int cols);
    explicit QuCircuit(QuArchitecture& architecture);
    ~QuCircuit();

    void run();

    friend std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit);

    void init1();
    void init2();

    int getLayerForNewGate(int gates[3], int operands);
    bool somethingInBetween(int row1, int row2, int layer);
    void initQuBitMappings(int **couplingMap);
    void findShortestPathsFor1InputMapping(shared_ptr<QuGate> quGate, int **couplingMap);
    void initializeMappings();
    void initializeMappings(QuArchitecture& quArchitecture);
    void printGrid();
    pair<int, QuMapping> findTotalSwaps(QuArchitecture& quArchitecture);
    vector<int> swapAlongPath(int *parent, int source, int destination);
    void printInstructions();

//    void build(string fileName);
    void build(string fileName);
    int getRows() const;
    void setCols(int cols);
    void setGrid(shared_ptr<QuGate>** grid);

    int getN() const;

    void setN(int n);

    void setInstructions(const vector<shared_ptr<QuGate>> instructions);

    void setFileName(const string &fileName);

    vector<shared_ptr<QuGate>>& getInstructions1();
    vector<shared_ptr<QuGate>> getInstructions() const;

    QuMapping& getMapping();
//
//    vector<int> &getSwapPath();
    void setSimpleGrid(int **simpleGrid);

    void printSimpleGrid();

    const string &getFileName() const;

    void setInstructions1(const vector<shared_ptr<QuGate>> & instructions1);

    int getHadamards() const;

    void setHadamards(int hadamards);

    int getSwaps() const;

    void setSwaps(int swaps);

    const vector<int> &getSrcFrequencies() const;

    void setSrcFrequencies(const vector<int> &srcFrequencies);

    const vector<int> &getDestFrequencies() const;

    void setDestFrequencies(const vector<int> &destFrequencies);

    void findCostFor1Instruction(shared_ptr<QuGate> quGate, int **couplingMap);

    int getOptimizations() const;

    void setOptimizations(int optimizations);
};

#endif //UCFQUSIM_QUCIRCUIT_H
