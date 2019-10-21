//
// Created by hammad on 9/26/19.
//

#include <iostream>
#include <fstream>
#include "QuCircuit.h"
#include "QuGateFactory.h"

using namespace std;

QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), quBitConfiguration(NULL), quBitMapping(NULL), grid(NULL) {
    init1();
    init2();
}

QuCircuit::~QuCircuit() {
    delete [] quBits;
    delete [] quBitConfiguration;
    delete [] quBitMapping;
    delete [] quBitLastLayer;
    for(int i=0; i<rows; i++)
        delete [] grid[i];
    delete [] grid;
}


void QuCircuit::add(QuGate* gate, int row, int depth) {
    grid[row][depth] = gate;
}

void QuCircuit::run() {
    std::cout << "Circuit execution start..." << std::endl;
    // todo: apply each gate depth-wise
    std::cout << "Circuit execution end..." << std::endl;
}

void QuCircuit::addMapping(int logicalQuBit, int physicalQuBit) {
    quBitMapping[logicalQuBit] = physicalQuBit;
}

std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit) {
    int temp = 0;
    for (int i = 0; i < circuit.rows; i++) {
        os << "q" << i << " : ";
        for (int j = 0; j < circuit.cols; j++) {
            if (circuit.grid[i][j] != NULL) {
                for (int k = temp+1; k < j; k++) {
                    os << "-";
                }
                os << circuit.grid[i][j]->getSymbol();
                temp = j;
            }
        }
        os << std::endl;
    }
    return os;
}

QuCircuit::QuCircuit(string fileName, int rows): rows(rows), cols(0) {
    ifstream ifs;
    string quGate = "";
    string qubitArgs = "";
    int qubitArgsInt[QuGate::MAX_OPERANDS] = {-1, -1, -1};
    int pos1 = 0;
    int pos2 = 0;
    int i = 0;

    init1(); // make dynamic arrays
    ifs.open(fileName);
    while (!ifs.eof()){
        i = 0;
        pos1 = 0;
        pos2 = 0;
        ifs >> quGate;
        ifs >> qubitArgs;
        cout << ">>>> " << quGate << " --- " << qubitArgs << endl;
        if((quGate.find("[") != string::npos) || (qubitArgs.find("[") != string::npos)) {
            cols++; // add layer as in worst case #layers == #gates
            while (pos1 != string::npos) {
                pos1 = qubitArgs.find("[", pos1 + 1);
                pos2 = qubitArgs.find("]", pos1 + 1);
                if ((pos1 != string::npos) && (pos2 != string::npos)) {
                    cout << ">>>> " << stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1)) << " <<<<" << endl;
                    qubitArgsInt[i++] = stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1));
                }
            }
            for (int j = 0; j < QuGate::MAX_OPERANDS; j++) {
                cout << qubitArgsInt[j] << endl;
                qubitArgsInt[j] = -1;
            }
            QuGate* newGate = QuGateFactory::getQuGate(quGate);
            setGateQubits(newGate, qubitArgsInt);
        }
    }
    ifs.close();

    init2(); // make the 2D circuit grid


}

void QuCircuit::init1() {
    quBits = new QuBit[rows];
    for (int j = 0; j<rows; j++) {
        quBits[j].setId(j);
    }
    quBitConfiguration = new int[rows];
    for(int i=0; i<rows; i++)
        quBitConfiguration[i] = i; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
    quBitMapping = new int[rows];
    quBitLastLayer = new int[rows] {-1};
    for (int j = 0; j<rows; j++) {
        quBitLastLayer[j] = -1;
    }
}


void QuCircuit::init2() {
    grid = new QuGate**[rows];
    for(int i=0; i<rows; i++)
        grid[i] = new QuGate*[cols];
}

void QuCircuit::setGateQubits(QuGate* newGatePtr, int* qubitArgsInt) {
//    for (int j = 0; j < QuGate::MAX_OPERANDS; j++) {
//        newGatePtr[j] = new QuBit
//        qubitArgsInt[j] != -1){
//
//        }
//    }
//    quBitLastLayer
}
