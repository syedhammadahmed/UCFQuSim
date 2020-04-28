//
// Created by YUSHAH on 10/22/2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "QuCircuitGenerator.h"
#include "QuGateFactory.h"
#include "util/Util.h"

using namespace std;

//QuCircuitGenerator::QuCircuitGenerator(QuCircuit& circuit) : layer(-1), circuit(circuit) {
//    rows = circuit.getRows();
//    quBitRecentLayer = new int[rows];
//    for(int i=0; i<rows; i++)
//        quBitRecentLayer[i] = -1;
//}

QuCircuitGenerator::QuCircuitGenerator(QuArchitecture &architecture):circuit(architecture), architecture(architecture), layer(-1) {
//    circuit = new QuCircuit(architecture);
    rows = architecture.getN();
    quBitRecentLayer = new int[rows];
    for(int i=0; i<rows; i++)
        quBitRecentLayer[i] = -1;
}

QuCircuitGenerator::QuCircuitGenerator(QuArchitecture &architecture, string inputFileAbsPath):circuit(architecture), architecture(architecture), layer(-1), inputFileAbsPath(inputFileAbsPath) {
//    circuit = new QuCircuit(architecture);
    rows = architecture.getN();
    quBitRecentLayer = new int[rows];
    for(int i=0; i<rows; i++)
        quBitRecentLayer[i] = -1;
    buildFromFile(inputFileAbsPath);
}


//
//QuCircuitGenerator::QuCircuitGenerator(QuCircuit &circuit, QuArchitecture &architecture):circuit(circuit), architecture(architecture), layer(-1) {
//    rows = circuit.getRows();
//    quBitRecentLayer = new int[rows];
//    for(int i=0; i<rows; i++)
//        quBitRecentLayer[i] = -1;
//}


void QuCircuitGenerator::buildFromFile(string fileName) {
    ifstream ifs;
    string quGate = "";
    string qubitArgs = "";
    int operandIndexes[QuGate::MAX_OPERANDS] = {-1, -1, -1};
    int pos1 = 0;
    int pos2 = 0;
    int i = 0;
    cols = 0;

//    int layer = -1;

    ifs.open(fileName);
    while (!ifs.eof()){
        i = 0;
        pos1 = 0;
        pos2 = 0;
        string line;
        getline(ifs, line);
//        cout << ">>> " << line << "<<<\t\t";
        if(line == "") continue;
        stringstream lineStream(line);
        getline(lineStream, quGate, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
        qubitArgs = line.substr(quGate.length()); // operands for the qu-gate - can be 1, 2, or 3 [simulator doesn't supports 4-operand qu-gates]
//        cout << ">>>> " << quGate << " --- " << qubitArgs << endl;
        if((quGate.find("[") != string::npos) || (qubitArgs.find("[") != string::npos)) {
            while (pos1 != string::npos) {
                pos1 = qubitArgs.find("[", pos1 + 1);
                pos2 = qubitArgs.find("]", pos1 + 1);
                if ((pos1 != string::npos) && (pos2 != string::npos)) {
//                    cout << ">>>> " << stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1)) << " <<<<" << endl;
                    operandIndexes[i++] = stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1));
                }
            }

            if(quGate != "qreg" && quGate != "creg" && quGate != "measure" && quGate != "rz" && quGate.substr(0,2) != "rz") {
                QuGate *newGate = QuGateFactory::getQuGate(quGate);
                for (int j = 0; j < newGate -> getCardinality(); j++) { // set gate operand qubits
                    int* arr = newGate->getArgIndex();
                    arr[j] = operandIndexes[j];
                }
//                layer++;
//                layer = getLayerForNewGate(operandIndexes, newGate -> getCardinality());
//                add(newGate, layer); // adds to grid
                instructions.push_back(newGate);
                cols++;
            }
        }
    }
    ifs.close();
    circuit.setCols(cols);
//    circuit.init2();
    circuit.setInstructions(instructions);
    buildGrid();
}

void QuCircuitGenerator::buildGrid() {
    init2(); // make grid
    try {
        for (QuGate *newGate: instructions) {
            for (int j = 0; j < newGate -> getCardinality(); j++) { // set gate operand qubits
                layer = getLayerForNewGate(newGate->getArgIndex(), newGate -> getCardinality());
                add(newGate, layer); // adds to grid
            }
        }
        circuit.setGrid(grid);
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

void QuCircuitGenerator::add(QuGate* gate, int depth) {
    int* quBitIndexes = gate -> getArgIndex();
    grid[quBitIndexes[0]][depth] = gate;
    if(gate -> getCardinality() == 2) {
        grid[quBitIndexes[1]][depth] = gate;
    }
}

int QuCircuitGenerator::getLayerForNewGate(int* gates, int operands) {
    int layer = 0;
    int max = quBitRecentLayer[gates[0]];
    for(int i = 1; i < operands; i++){
        if(quBitRecentLayer[gates[i]] > max)
            max = quBitRecentLayer[gates[i]];
    }
    layer = max + 1;
    for(int i = 0; i < operands; i++) {
        quBitRecentLayer[gates[i]] = layer;
    }

//    if(operands > 1) {
//        // if a gate is b/w a binary S and T qbits of the new gate in the same layer
//        max = gates[0];
//        int min = gates[0];
//        for (int i = 1; i < operands; i++) {
//            if (gates[i] > max) max = gates[i];
//            if (gates[i] < min) min = gates[i];
//        }
//
//        while (somethingInBetween(min + 1, max - 1, layer)) {
//            layer++;
//            for (int i = 0; i < operands; i++) {
//                quBitRecentLayer[gates[i]] = layer;
//            }
//        }
//
//        //    for(int i = 0; i < operands; i++) {
//        //        quBitRecentLayer[gates[i]] = layer;
//        //    }
//    }
    return layer;
}

bool QuCircuitGenerator::somethingInBetween(int row1, int row2, int layer) {
    for (int i = row1; i <= row2; i++)
        if (grid[i][layer] != NULL)
            return true;
    return false;
}

void QuCircuitGenerator::makeProgramFile(string outputFileName) {
    ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    try {
        for (QuGate *quGate: instructions) {
            string instruction = Util::toLower(quGate->getMnemonic()) + " q[" + to_string(quGate->getArgIndex()[0]) + "]";
            if (quGate->getCardinality() > 1)
                instruction += ", q[" + to_string(quGate->getArgIndex()[1]) + "]";
            instruction += ";";
            ofs << instruction << endl;
        }
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

QuCircuitGenerator::~QuCircuitGenerator() {
    delete [] quBitRecentLayer;
    for(int i = 0; i < rows; i++)
        delete [] grid[i];
    delete [] grid;
    for(QuGate* ptr: instructions){
        delete ptr;
    }
}

// initializes the circuit grid
void QuCircuitGenerator::init2() {
    grid = new QuGate**[rows];
    for(int i = 0; i < rows; i++)
        grid[i] = new QuGate*[cols];
    for(int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            grid[i][j] = NULL;
//    printGrid();
}

void QuCircuitGenerator::setInstructions(const vector<QuGate*> instructions) {
    this -> instructions = instructions;
}

const vector<QuGate*> QuCircuitGenerator::getInstructions() const{
    return instructions;
}

int QuCircuitGenerator::getLayer() const {
    return layer;
}

QuCircuit& QuCircuitGenerator::getCircuit() {
    return circuit;
}

