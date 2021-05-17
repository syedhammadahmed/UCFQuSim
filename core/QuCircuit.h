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

    vector<int> qubits;

public:
    QuCircuit();
    explicit QuCircuit(int rows);
    explicit QuCircuit(string file, int rows);

    vector<shared_ptr<QuGate>> getBinaryInstructions();
    vector<shared_ptr<QuGate>> getKBinaryInstructions(int k);

    void run();

    void printGrid();
    void printSimpleGrid();
    void printInstructions();
    friend std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit);

    // setters and getters
    void setCols(int cols);
    void setN(int n);
    void setGrid(shared_ptr<QuGate>** grid);
    void setFileName(const string &fileName);

    const string &getFileName() const;

    void setSimpleGrid(int **simpleGrid);
    void setInstructions0(const vector<shared_ptr<QuGate>>& instructions0);
    void setInstructions1(const vector<shared_ptr<QuGate>>& instructions1);
    void setHadamards(int hadamards);
    void setSwaps(int swaps);
    void setSrcFrequencies(const vector<int> &srcFrequencies);
    void setDestFrequencies(const vector<int> &destFrequencies);
    void setOptimizations(int optimizations);

    int getRows() const;
    int getN() const;
    int getHadamards() const;
    vector<shared_ptr<QuGate>>& getInstructions0();
    vector<shared_ptr<QuGate>>& getInstructions1();
    int getOptimizations() const;
    int getSwaps() const;

    shared_ptr<QuGate> **getGrid() const;


    void setQubits(vector<int> qubits);

    vector<int> getQubits();

    vector<shared_ptr<QuGate>> getKUniqueBinaryInstructions(int k);
};

#endif //UCFQUSIM_QUCIRCUIT_H
