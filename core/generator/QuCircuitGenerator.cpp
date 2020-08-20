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

//            if(quGate != "qreg" && quGate != "creg" && quGate != "measure" && quGate != "rz" && quGate.substr(0,2) != "rz") {
            if(quGate != "qreg" && quGate != "creg" && quGate != "measure") {
                if(quGate.substr(0,2) == "rz")
                    quGate = "rz";
                QuGate *newGate = QuGateFactory::getQuGate(quGate);
                for (int j = 0; j < newGate -> getCardinality(); j++) { // set gate operand qubits
                    newGate->setArgAtIndex(j, operandIndexes[j]);
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
    circuit.setInstructions(instructions);
    circuit.setFileName(fileName);
    buildGrid();
//    QuGate*** grid;
//    QuMapping mapping;

}


//
//QuGate* QuCircuitGenerator::parseInstruction(string line){
//    QuGate* newGate = nullptr;
//    string quGate = "", qubitArgs = "";
//
//    if(!line.empty()) {
//        stringstream lineStream(line);
//        getline(lineStream, quGate, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
//        qubitArgs = line.substr(quGate.length()); // operands for the qu-gate - can be 1, 2, or 3 [simulator doesn't supports 4-operand qu-gates]
//    //        cout << ">>>> " << quGate << " --- " << qubitArgs << endl;
////        if((quGate.find("[") != string::npos) || (qubitArgs.find("[") != string::npos)) {
//        int pos1 = qubitArgs.find("[");
//        int pos2 = qubitArgs.find("]", pos1 + 1);
//            while (pos1 != string::npos) {
////                pos2 = qubitArgs.find("[", pos1 + 1);
//                if ((pos1 != string::npos) && (pos2 != string::npos)) {
//    //                    cout << ">>>> " << stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1)) << " <<<<" << endl;
//                    operandIndexes[i++] = stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1));
//                }
//            }
//
//            if(quGate != "qreg" && quGate != "creg" && quGate != "measure" && quGate != "rz" && quGate.substr(0,2) != "rz") {
//                QuGate *newGate = QuGateFactory::getQuGate(quGate);
//                for (int j = 0; j < newGate -> getCardinality(); j++) { // set gate operand qubits
//                    int* arr = newGate->getArgIndex();
//                    arr[j] = operandIndexes[j];
//                }
//    //                layer++;
//    //                layer = getLayerForNewGate(operandIndexes, newGate -> getCardinality());
//    //                add(newGate, layer); // adds to grid
//                instructions.push_back(newGate);
//                cols++;
//            }
//    }
//
//    return newGate;
//}


void QuCircuitGenerator::buildGrid() {
    init2(); // make grid
    try {
        int currentInstruction = 0;
        for (QuGate *newGate: instructions) {
            int operands = newGate -> getCardinality();
            vector<int> args = newGate -> getArgIndex();
//            for (int j = 0; j < operands; j++) { // set gate operand qubits
                layer = getLayerForNewGate(args, operands);
                add(newGate, layer); // adds to grid
                addSimple(newGate, layer, currentInstruction);
//            }
            currentInstruction++;
        }
        circuit.setGrid(grid);
        circuit.setSimpleGrid(simpleGrid);
//        circuit.printGrid();
//        circuit.printSimpleGrid();
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

void QuCircuitGenerator::add(QuGate* gate, int depth) {
    vector<int> quBits = gate -> getArgIndex();
    grid[quBits[0]][depth] = gate;
    if(gate -> getCardinality() > 1) {
        for(int i = 1; i < gate->getCardinality(); i++) {
            QuGate* temp = QuGateFactory::getQuGate(gate->getMnemonic());
            temp->setPrintIndex(i);
            grid[quBits[i]][depth] = temp;
        }
    }
}

int QuCircuitGenerator::getLayerForNewGate(vector<int> quBits, int operands) {
    int layer = 0;
    int max = quBitRecentLayer[quBits[0]];
    for(int i = 1; i < operands; i++){
        if(quBitRecentLayer[quBits[i]] > max)
            max = quBitRecentLayer[quBits[i]];
    }
    layer = max + 1;
    while(operands>1 && somethingInBetween(quBits, operands, layer))
        layer++;
    for(int i = 0; i < operands; i++) {
        quBitRecentLayer[quBits[i]] = layer;
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

//bool QuCircuitGenerator::somethingInBetween(int row1, int row2, int layer) {
//    for (int i = row1; i <= row2; i++)
//        if (grid[i][layer] != NULL)
//            return true;
//    return false;
//}
bool QuCircuitGenerator::somethingInBetween(int row1, int row2, int layer) {
    if (row1 > row2)
        swap(row1, row2);

    for (int i = row1+1; i < row2; i++)
        if (simpleGrid[i][layer] != -1)
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
        for (int j = 0; j < cols; j++)
            if(grid[i][j] != NULL)
                delete grid[i][j];  // deleting the gate made by QuGateFactory

    for(int i = 0; i < rows; i++)
        delete [] grid[i];
    delete [] grid;
    for(int i = 0; i < rows; i++)
        delete [] simpleGrid[i];

    delete [] simpleGrid;
//    for(QuGate* ptr: instructions){
//        if(ptr != nullptr)
//            delete ptr;
//    }
}

// initializes the circuit grid
void QuCircuitGenerator::init2() {
    grid = new QuGate**[rows];
    for(int i = 0; i < rows; i++)
        grid[i] = new QuGate*[cols];
    for(int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            grid[i][j] = NULL;

    simpleGrid = new int*[rows];
    for(int i = 0; i < rows; i++)
        simpleGrid[i] = new int[cols];
    for(int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            simpleGrid[i][j] = -1;

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

void QuCircuitGenerator::addSimple(QuGate *gate, int depth, int instructionNo) {
    vector<int> quBits = gate -> getArgIndex();
    int cardinality = gate -> getCardinality();
    for(int i = 0; i < cardinality; i++)
        simpleGrid[quBits[i]][depth] = instructionNo;
}

bool QuCircuitGenerator::somethingInBetween(vector<int> quBits, int operands, int layer) {
    for(int i = 0; i < operands; i++)
        for(int j = i + 1; j < operands; j++){
            if (somethingInBetween(quBits[i], quBits[j], layer))
                return true;
        }
    return false;
}

