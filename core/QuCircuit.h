//
// Created by hammad on 9/26/19.
//

#ifndef UCFQUSIM_QUCIRCUIT_H
#define UCFQUSIM_QUCIRCUIT_H

#include <vector>
#include <ostream>
#include <string>
#include "gates/QuGate.h"
#include "QuInstruction.h"
#include "QuMapping.h"
#include "QuArchitecture.h"

using namespace std;

class QuCircuit {

private:
    int rows; // # of physical quBits (max rows)
    int cols; // depth
//    int logicalToPhysicalMapping[16]; // logical to physical mapping of quBits : logical index -> physical elements
//    int physicalToLogicalMapping[16]; // physical to logical mapping of quBits : physical index -> logical elements
//    int* logicalToPhysicalMapping; // logical to physical mapping of quBits : logical index -> physical elements
//    int* physicalToLogicalMapping; // physical to logical mapping of quBits : physical index -> logical elements
//    int* quBitRecentLayer;
//    QuBit* logicalQuBits;
    QuGate*** grid;
    vector<QuGate*> instructions; // qasm program instructions/qugates
    vector<QuGate*> instructionsV1; // modified qasm program after inserting swap and H instructions/qugates

    string fileName; // circuit input file name (absolute path)
    QuMapping mapping;
//    vector<int> swapPath;

public:
    QuCircuit();
    QuCircuit(int rows);
    QuCircuit(int rows, int cols);
//    QuCircuit(string fileName, int rows);
    QuCircuit(QuArchitecture& architecture);

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
    int findSwapsFor1Instruction(QuGate* quGate, int **couplingMap);
    void initializeMappings();
    void initializeMappings(QuArchitecture& quArchitecture);
    void printMappings();
    void printGrid();
    int findTotalSwaps(QuArchitecture& quArchitecture);
    vector<int> swapAlongPath(int *parent, int source, int destination);
    void printInstructions();

//    void build(string fileName);
    void build(string fileName);
    int getRows() const;
    void setCols(int cols);
    void setGrid(QuGate ***grid);
    void setInstructions(const vector<QuGate*> instructions);

    void setFileName(const string &fileName);

    vector<QuGate*>& getInstructionsV1();
    vector<QuGate*> getInstructions() const;

    QuMapping& getMapping();
//
//    vector<int> &getSwapPath();

};

#endif //UCFQUSIM_QUCIRCUIT_H
