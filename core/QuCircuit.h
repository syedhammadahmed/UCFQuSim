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
    int rows; // # of physical quBits (max rows)
    int cols; // depth
    int n; // # of logical qubits
//    int logicalToPhysicalMapping[16]; // logical to physical mapping of quBits : logical index -> physical elements
//    int physicalToLogicalMapping[16]; // physical to logical mapping of quBits : physical index -> logical elements
//    int* logicalToPhysicalMapping; // logical to physical mapping of quBits : logical index -> physical elements
//    int* physicalToLogicalMapping; // physical to logical mapping of quBits : physical index -> logical elements
//    int* quBitRecentLayer;
//    QuBit* logicalQuBits;
    QuGate*** grid;
    int** simpleGrid;
    vector<QuGate*> instructions; // qasm program instructions/qugates
    vector<QuGate*> instructionsV1; // modified qasm program after inserting swap and H instructions/qugates

    string fileName; // circuit input file name (absolute path)
    QuMapping mapping;
    int hadamards;
    int swaps;
    int cost;
    int optimizations;

    vector<int> srcFrequencies;
    vector<int> destFrequencies;
public:
    QuCircuit();
    QuCircuit(int rows);
    QuCircuit(int rows, int cols);
//    QuCircuit(string fileName, int rows);
    QuCircuit(QuArchitecture& architecture);
    QuCircuit(const QuCircuit& arg);


    void add(QuGate* gate, int row, int depth);
    void add(QuGate* gate, int depth);
    void addMapping(int logicalQuBit, int physicalQuBit);
    void run();
//    QuGate* operator[][](int, int);

    friend std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit);

    virtual ~QuCircuit();

    void init1();
    void init2();

    int getLayerForNewGate(int gates[3], int operands);
    bool somethingInBetween(int row1, int row2, int layer);
    void initQuBitMappings(int **couplingMap);
    void findShortestPathsFor1InputMapping(QuGate* quGate, int **couplingMap);
    void initializeMappings();
    void initializeMappings(QuArchitecture& quArchitecture);
    void printMappings();
    void printGrid();
    pair<int, QuMapping> findTotalSwaps(QuArchitecture& quArchitecture);
    vector<int> swapAlongPath(int *parent, int source, int destination);
    void printInstructions();

//    void build(string fileName);
    void build(string fileName);
    int getRows() const;
    void setCols(int cols);
    void setGrid(QuGate ***grid);

    int getN() const;

    void setN(int n);

    void setInstructions(const vector<QuGate*> instructions);

    void setFileName(const string &fileName);

    vector<QuGate*>& getInstructionsV1();
    vector<QuGate*> getInstructions() const;

    QuMapping& getMapping();
//
//    vector<int> &getSwapPath();
    void setSimpleGrid(int **simpleGrid);

    void printSimpleGrid();

    const string &getFileName() const;

    void setInstructionsV1(const vector<QuGate*> & instructionsV1);

    int getHadamards() const;

    void setHadamards(int hadamards);

    int getSwaps() const;

    void setSwaps(int swaps);

    const vector<int> &getSrcFrequencies() const;

    void setSrcFrequencies(const vector<int> &srcFrequencies);

    const vector<int> &getDestFrequencies() const;

    void setDestFrequencies(const vector<int> &destFrequencies);

    void findCostFor1Instruction(QuGate *quGate, int **couplingMap);

    int getOptimizations() const;

    void setOptimizations(int optimizations);
};

#endif //UCFQUSIM_QUCIRCUIT_H
