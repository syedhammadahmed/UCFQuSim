//
// Created by SHA on 9/26/19.
//

#include <iostream>
#include <iomanip>
#include "generator/QuSwapStrategy.h"
#include "generator/QuSmartSwapper.h"
#include "QuCircuit.h"
#include "QuGateFactory.h"
#include "util/Util.h"
#include "QuArchitecture.h"

using namespace std;

QuCircuit::QuCircuit() {
}

QuCircuit::QuCircuit(int rows): rows(rows), grid(NULL) {
}

void QuCircuit::run() {
    if(Util::verbose)
        cout << "Circuit execution start..." << endl;
    // todo: apply each gate depth-wise
    if(Util::verbose)
        cout << "Circuit execution end..." << endl;
}

void QuCircuit::printGrid(){
    if(Util::verbose) {
        cout << "Printing Circuit Grid (grid of pointers to qugates): " << endl;
        for (int i = 0; i < 50; i++) cout << "__";   // todo make it generic for any layers
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << std::setw(8);
            cout << "q[" << i << "]";
            for (int j = 0; j < cols; j++) {
                string symbol = ".";
                if (grid[i][j] != NULL) {
                    symbol = grid[i][j]->getPrintSymbol();
                }
                cout << std::setw(4);
                cout << symbol << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < 50; i++) cout << "__";
        cout << endl;
    }
}

void QuCircuit::printSimpleGrid(){
    if(Util::verbose) {
        cout << "Printing Circuit Grid (grid of qugates positions): " << endl;
        for (int i = 0; i < 50; i++) cout << "__";   // todo make it generic for any layers
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << std::setw(8);
            cout << "q[" << i << "]";
            for (int j = 0; j < cols; j++) {
                cout << std::setw(4);
                if (simpleGrid[i][j] == -1)
                    cout << "." << " ";
                else
                    cout << simpleGrid[i][j] << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < 50; i++) cout << "__";
        cout << endl;
    }
}

void QuCircuit::printInstructions() {
    if(Util::verbose) {
        try {
            for (shared_ptr<QuGate> quGate: instructions1) {
                cout << *quGate << endl;
            }
        } catch (exception &e) {
            cout << "Exception : " << e.what() << '\n';

        }
    }
}

ostream &operator<<(ostream &os, const QuCircuit &circuit) {
    int temp = 0;
    for (int i = 0; i < circuit.rows; i++) {
        os << "q" << i << " : ";
        for (int j = 0; j < circuit.cols; j++) {
            shared_ptr<QuGate> gate = circuit.grid[i][j];
            if (gate != NULL) {
                if(gate -> getCardinality() > 1) {

                    int s = gate->getArgAtIndex(0);
                    int t = gate->getArgAtIndex(1);
                    if (i == t)
                        circuit.grid[i][j]->setPrintIndex(1);
                    else
                        circuit.grid[i][j]->setPrintIndex(0);
                }
                os << circuit.grid[i][j] -> getPrintSymbol() << " ";
            } else
                os << "~ ";
        }
        os << endl;
    }
    return os;
}


int QuCircuit::getRows() const {
    return rows;
}

void QuCircuit::setCols(int cols) {
    this -> cols = cols;
}

void QuCircuit::setGrid(shared_ptr<QuGate>** grid) {
    this -> grid = std::move(grid);
}

void QuCircuit::setInstructions0(const vector<shared_ptr<QuGate>> instructions) {
    this -> instructions0 = instructions;
}

vector<shared_ptr<QuGate>>& QuCircuit::getInstructions1(){
    return instructions1;
}

void QuCircuit::setInstructions1(const vector<shared_ptr<QuGate>>& instructions1) {
    this->instructions1 = instructions1;
}

vector<shared_ptr<QuGate>> QuCircuit::getInstructions0() {
    return instructions0;
}

void QuCircuit::setFileName(const string &fileName) {
    this->fileName = fileName;
}

void QuCircuit::setSimpleGrid(int** simpleGrid) {
    this->simpleGrid = simpleGrid;
}

int QuCircuit::getHadamards() const {
    return hadamards;
}

void QuCircuit::setHadamards(int hadamards) {
    QuCircuit::hadamards = hadamards;
}

int QuCircuit::getSwaps() const {
    return swaps;
}

void QuCircuit::setSwaps(int swaps) {
    this->swaps = swaps;
}

int QuCircuit::getN() const {
    return n;
}

void QuCircuit::setN(int n) {
    this->n = n;
}

void QuCircuit::setSrcFrequencies(const vector<int> &srcFrequencies) {
    this->srcFrequencies = srcFrequencies;
}

void QuCircuit::setDestFrequencies(const vector<int> &destFrequencies) {
    this->destFrequencies = destFrequencies;
}

int QuCircuit::getOptimizations() const {
    return optimizations;
}

void QuCircuit::setOptimizations(int optimizations) {
    this->optimizations = optimizations;
}



