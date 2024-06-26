//
// Created by SHA on 10/22/2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <util/Constants.h>
#include "Config.h"
#include "QuCircuitGenerator.h"
#include "QuGateFactory.h"
#include "util/Util.h"

using namespace std;

QuCircuitGenerator::QuCircuitGenerator(int n, vector<shared_ptr<QuGate>> instructions): n(n), circuit(n), fromFile(false) {
    quBitRecentLayer = new int[n];
    for(int i=0; i<n; i++)
        quBitRecentLayer[i] = -1;
    circuit.setInstructions0(instructions);

}

QuCircuitGenerator::QuCircuitGenerator(int n, string fileName):n(n), circuit(fileName, n), layer(-1), fromFile(true) {
    quBitRecentLayer = new int[n];
    for(int i=0; i<n; i++)
        quBitRecentLayer[i] = -1;
    buildFromFile(fileName);
}

QuCircuitGenerator::~QuCircuitGenerator() {
    delete [] quBitRecentLayer;
    if (CIRCUIT_BUILD_GRID) {
        deleteGrids();
    }
}

void QuCircuitGenerator::buildFromFile(string fileName) {
    ifstream ifs;
    string quGate = "";
    string qubitArgs = "";
    int operandIndexes[QuGate::MAX_OPERANDS] = {-1, -1, -1};
    int pos1 = 0;
    int pos2 = 0;
    int i = 0;
    cols = 0;
    fromFile = true;
    vector<int> qubits;
    vector<shared_ptr<QuGate>> instructions; // original qasm program instructions/qugates

    vector<pair<int ,int>> srcFrequencies(n, make_pair(-1, 0));
    vector<pair<int ,int>> destFrequencies(n, make_pair(-1, 0));
    vector<pair<int ,int>> commons(n);

    std::vector<pair<int ,int>>::iterator it;
//    int layer = -1;
    int duals = 0;
    header = "";
    string theFileName = Constants::INPUT_FILES_DIRECTORY_RPATH + fileName + Constants::FILE_EXTENSION;
    ifs.open(theFileName);
    while (!ifs.eof() && quGate != "creg") {
        string line;
        getline(ifs, line);
        header += line + '\n';
        if(line == "") continue;
        stringstream lineStream(line);
        getline(lineStream, quGate, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
    }

    while (!ifs.eof()){
        i = 0;
        pos1 = 0;
        string line;
        getline(ifs, line);
        if(line == "") continue;
        stringstream lineStream(line);
        getline(lineStream, quGate, ' '); // mnemonic for qu-gate e.g. h for Hadamard, x for NOT, cx for C-NOT, etc.
        qubitArgs = line.substr(quGate.length()); // operands for the qu-gate - can be 1, 2, or 3 [simulator doesn't supports 4-operand qu-gates]
        if((quGate.find("[") != string::npos) || (qubitArgs.find("[") != string::npos)) {
            while (pos1 != string::npos) {
                pos1 = qubitArgs.find("[", pos1 + 1);
                pos2 = qubitArgs.find("]", pos1 + 1);
                if ((pos1 != string::npos) && (pos2 != string::npos)) {
                    operandIndexes[i++] = stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1));
                }
            }

            if(quGate != "qreg" && quGate != "creg" && quGate != "measure") {
                string theta;
                if(quGate.substr(0,2) == "rz") {
                    pos1 = quGate.find("(");
                    pos2 = quGate.find(")");
                    theta = quGate.substr(pos1 + 1, pos2 - pos1 - 1);
                    quGate = "rz";
                }
                shared_ptr<QuGate> newGate = std::move(QuGateFactory::getQuGate(quGate));
                for (int j = 0; j < newGate -> getCardinality(); j++) { // set gate operand qubits
                    newGate->setArgAtIndex(j, operandIndexes[j]);
                    newGate->setTheta(theta); // for rz
                    qubits.push_back(operandIndexes[j]); // to find unique logical qubits used in program
                    // for ranking todo: not used -- may remove
                    if (INIT_MAPPING_START_NODE_RANK_WISE) {
                        if (j == 0 && newGate->getCardinality() > 1 && duals < K) {
                            srcFrequencies[operandIndexes[j]].first = operandIndexes[j];
                            srcFrequencies[operandIndexes[j]].second++;
                        }
                        if (j == 1 && duals < K) {
                            destFrequencies[operandIndexes[j]].first = operandIndexes[j];
                            destFrequencies[operandIndexes[j]].second++;
                            duals++;
                        }
                    }
                }
//                layer++;
//                layer = getLayerForNewGate(operandIndexes, newGate -> getCardinality());
//                add(newGate, layer); // adds to grid
                newGate->setGateId(cols);
                instructions.push_back(newGate);
                cols++;
            }
        }
    }
    ifs.close();
    circuit.setCols(cols);
    circuit.setInstructions0(instructions);
    circuit.setFileName(fileName);
    if(CIRCUIT_BUILD_GRID)
        buildGrid();

    std::sort(qubits.begin(), qubits.end());
    auto qit = std::unique(qubits.begin(), qubits.begin() + qubits.size());
    qubits.resize(std::distance(qubits.begin(),qit));

    int n = qubits.size();
    circuit.setN(n);
    circuit.setQubits(qubits);

    // for ranking todo: not used -- may remove
    if (INIT_MAPPING_START_NODE_RANK_WISE) {
        sort(srcFrequencies.begin(), srcFrequencies.end(), Util::sortBySecDesc);
        sort(destFrequencies.begin(), destFrequencies.end(), Util::sortBySecDesc);

        vector<int> freq;
        for (auto const &x: srcFrequencies)
            freq.push_back(x.second);
        circuit.setSrcFrequencies(freq);
        freq.clear();
        for (auto const &x: destFrequencies)
            freq.push_back(x.second);
        circuit.setDestFrequencies(freq);
    }
}

void QuCircuitGenerator::buildGrid() {
    auto instructions = circuit.getInstructions0();
    if (!fromFile)
        cols = instructions.size();
    initGrids(); // make grid
    try {
        int currentInstruction = 0;
        for (shared_ptr<QuGate> newGate: instructions) {
            int operands = newGate -> getCardinality();
            vector<int> args = newGate -> getArgIndex();
            layer = getLayerForNewGate(args, operands);
            add(newGate, layer); // adds to grid
            addSimple(newGate, layer, currentInstruction);
            currentInstruction++;
        }
        circuit.setGrid(grid);
        circuit.setSimpleGrid(simpleGrid);
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

// initializes the circuit grid
void QuCircuitGenerator::initGrids() {
    grid = new shared_ptr<QuGate> *[n];
    for (int i = 0; i < n; i++)
        grid[i] = new shared_ptr<QuGate>[cols];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < cols; j++)
            grid[i][j] = NULL;

    simpleGrid = new int *[n];
    for (int i = 0; i < n; i++)
        simpleGrid[i] = new int[cols];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < cols; j++)
            simpleGrid[i][j] = -1;
    //    printGrid();
}

void QuCircuitGenerator::add(shared_ptr<QuGate> gate, int depth) {
    vector<int> quBits = gate -> getArgIndex();
    grid[quBits[0]][depth] = gate;
    if(gate -> getCardinality() > 1) {
        for(int i = 1; i < gate->getCardinality(); i++) {
            shared_ptr<QuGate> temp = QuGateFactory::getQuGate(gate->getMnemonic());
            temp->setPrintIndex(i);
            grid[quBits[i]][depth] = temp;
        }
    }
}

void QuCircuitGenerator::addSimple(shared_ptr<QuGate> gate, int depth, int instructionNo) {
    vector<int> quBits = gate -> getArgIndex();
    int cardinality = gate -> getCardinality();
    for(int i = 0; i < cardinality; i++)
        simpleGrid[quBits[i]][depth] = instructionNo;
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
    return layer;
}

bool QuCircuitGenerator::somethingInBetween(int row1, int row2, int layer) {
    if (row1 > row2)
        swap(row1, row2);

    for (int i = row1+1; i < row2; i++)
        if (simpleGrid[i][layer] != -1)
            return true;
    return false;
}

bool QuCircuitGenerator::somethingInBetween(vector<int> quBits, int operands, int layer) {
    for(int i = 0; i < operands; i++)
        for(int j = i + 1; j < operands; j++){
            if (somethingInBetween(quBits[i], quBits[j], layer))
                return true;
        }
    return false;
}

void QuCircuitGenerator::deleteGrids() {
    for (int i = 0; i < n; i++)
        delete[] grid[i];
    delete[] grid;

    for (int i = 0; i < n; i++)
        delete[] simpleGrid[i];
    delete[] simpleGrid;
}

void QuCircuitGenerator::makeProgramFile(string outputFileName, vector<shared_ptr<QuGate>> finalProgram) {
    ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    try {
        ofs << header << endl;
        for (shared_ptr<QuGate> quGate: finalProgram) {
            string mnemonic = Util::toLower(quGate->getMnemonic());
            if(mnemonic == "rz")
                mnemonic += "(" + quGate->getTheta() + ")";
            string instruction = mnemonic + " q[" + to_string(quGate->getArgIndex()[0]) + "]";
            if (quGate->getCardinality() > 1)
                instruction += ",q[" + to_string(quGate->getArgIndex()[1]) + "]";
            instruction += ";";
            ofs << instruction << endl;
        }
        ofs.close();
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

void QuCircuitGenerator::makeProgramFile(string outputFileName) {
    ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    try {
        ofs << header << endl;
        vector<shared_ptr<QuGate>> instructions = circuit.getInstructions1();
        for (shared_ptr<QuGate> quGate: instructions) {
            string mnemonic = Util::toLower(quGate->getMnemonic());
            if(mnemonic == "rz")
                mnemonic += "(" + quGate->getTheta() + ")";
            string instruction = mnemonic + " q[" + to_string(quGate->getArgIndex()[0]) + "]";
            if (quGate->getCardinality() > 1)
                instruction += ",q[" + to_string(quGate->getArgIndex()[1]) + "]";
            instruction += ";";
            ofs << instruction << endl;
        }
        ofs.close();
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

int QuCircuitGenerator::getLayer() const {
    return layer;
}

QuCircuit& QuCircuitGenerator::getCircuit() {
    return circuit;
}

