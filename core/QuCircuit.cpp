//
// Created by hammad on 9/26/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "QuCircuit.h"
#include "QuGateFactory.h"
#include "../ShortestPathFinder.h"
#include "QuInstruction.h"
#include "Util.h"

using namespace std;

QuCircuit::QuCircuit(int rows): rows(rows), logicalToPhysicalMapping(NULL), physicalToLogicalMapping(NULL), grid(NULL) {
    init1();
}

QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), logicalToPhysicalMapping(NULL), physicalToLogicalMapping(NULL), grid(NULL) {
    init1();
}

/*
QuCircuit::QuCircuit(string fileName, int rows): rows(rows), cols(20), logicalToPhysicalMapping(NULL), physicalToLogicalMapping(NULL), grid(NULL) {
    ifstream ifs;
    string quGate = "";
    string qubitArgs = "";
    int gates[QuGate::MAX_OPERANDS] = {-1, -1, -1};
    int pos1 = 0;
    int pos2 = 0;
    int i = 0;
    int layer = -1;

    init1();
    init2();

    ifs.open(fileName);
    while (!ifs.eof()){
        i = 0;
        pos1 = 0;
        pos2 = 0;
        string line;
        getline(ifs, line);
        if(line == "") continue;
//        cout << "line #" << cols + 1 << ": " << line << " || ";
        stringstream lineStream(line);
        getline(lineStream, quGate, ' ');
        qubitArgs = line.substr(quGate.length());
//        cout << ">>>> " << quGate << " --- " << qubitArgs << endl;
        if((quGate.find("[") != string::npos) || (qubitArgs.find("[") != string::npos)) {
            while (pos1 != string::npos) {
                pos1 = qubitArgs.find("[", pos1 + 1);
                pos2 = qubitArgs.find("]", pos1 + 1);
                if ((pos1 != string::npos) && (pos2 != string::npos)) {
//                    cout << ">>>> " << stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1)) << " <<<<" << endl;
                    gates[i++] = stoi(qubitArgs.substr(pos1 + 1, pos2 - pos1 - 1));
                }
            }
            if(quGate != "qreg" && quGate != "creg") {
                QuGate *newGate = QuGateFactory::getQuGate(quGate);
                // set gate operand qubits
                for (int j = 0; j < newGate -> getCardinality(); j++) {
//                    cout << gates[j] << endl;
                    int* arr = newGate->getArgIndex();
                    arr[j] = gates[j];
//                    if (gates[j] != -1) {
//                        (*newGate)[j] = logicalQuBits[j];
//                    }
//                    gates[j] = -1;
                }
                layer++;
//                layer = getLayerForNewGate(gates, newGate -> getCardinality());
                add(newGate, layer);
                instructionsV0.push_back(newGate);
//                printGrid();
//                cout << "Added at layer : " << layer << " " << newGate -> getSymbol() << endl;
//                cols++;
            }
        }
    }
    ifs.close();
//    printGrid();
//    init2();
}
*/

//// initializes the circuit grid
//void QuCircuit::init2() {
//    grid = new QuGate**[rows];
//    for(int i = 0; i < rows; i++)
//        grid[i] = new QuGate*[cols];
//    for(int i = 0; i < rows; i++)
//        for (int j = 0; j < cols; j++)
//            grid[i][j] = NULL;
////    printGrid();
//}


QuCircuit::~QuCircuit() {
    delete [] logicalToPhysicalMapping;
    delete [] physicalToLogicalMapping;
    delete [] logicalQuBits;
//    for(int i=0; i<rows; i++)
//        delete [] grid[i];
//    delete [] grid;
}


//void QuCircuit::add(QuGate* gate, int row, int depth) {
//    grid[row][depth] = gate;
//}

//void QuCircuit::add(QuGate* gate, int depth) {
//    int* quBitIndexes = gate -> getArgIndex();
//    grid[quBitIndexes[0]][depth] = gate;
//    if(gate -> getCardinality() == 2) {
//        grid[quBitIndexes[1]][depth] = gate;
//    }
//}

void QuCircuit::run() {
    std::cout << "Circuit execution start..." << std::endl;
    // todo: apply each gate depth-wise
    std::cout << "Circuit execution end..." << std::endl;
}

void QuCircuit::addMapping(int logicalQuBit, int physicalQuBit) {
    logicalToPhysicalMapping[logicalQuBit] = physicalQuBit;
    physicalToLogicalMapping[physicalQuBit] = logicalQuBit;
}

std::ostream &operator<<(std::ostream &os, const QuCircuit &circuit) {
    int temp = 0;
    for (int i = 0; i < circuit.rows; i++) {
//        temp = 0;
        os << "q" << i << " : ";
        for (int j = 0; j < circuit.cols; j++) {
            QuGate* gate = circuit.grid[i][j];
            if (gate != NULL) {
//                for (int k = temp+1; k < j; k++) {
//                    os << "-";
//                }
                if(gate -> getCardinality() > 1) {
                    int *arr = gate->getArgIndex();
                    int s = arr[0];
                    int t = arr[1];
                    if (i == t)
                        circuit.grid[i][j]->setPrintIndex(1);
                    else
                        circuit.grid[i][j]->setPrintIndex(0);
                }
                os << circuit.grid[i][j] -> getPrintSymbol() << " ";


//                temp = j;
            } else
                os << "~ ";
        }
        os << std::endl;
    }
    return os;
}

// initializes the 1D arrays - qubits, mappings, etc.
void QuCircuit::init1() {
    logicalToPhysicalMapping = new int[rows];
    physicalToLogicalMapping = new int[rows];
    logicalQuBits = new QuBit[rows];
}

//// initializes the circuit grid
//void QuCircuit::init2() {
//    grid = new QuGate**[rows];
//    for(int i=0; i<rows; i++)
//        grid[i] = new QuGate*[cols];
//    for(int i=0; i<rows; i++)
//        for (int j = 0; j < cols; j++)
//            grid[i][j] = NULL;
////    printGrid();
//}

void QuCircuit::printGrid(){
    cout << "Printing Circuit Grid (grid of pointers to qugates): " << endl;
    for(int i=0; i<50; i++) cout << "__";
    cout << endl;
    for(int i=0; i<rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << grid[i][j] << " ";
        cout << endl;
    }
    for(int i=0; i<50; i++) cout << "__";
    cout << endl;
}

void QuCircuit::initializeMappings(int** couplingMap){
    if(couplingMap == NULL){
        for(int i=0; i<rows; i++) {
            logicalToPhysicalMapping[i] = i; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
            physicalToLogicalMapping[i] = i; // it may change due to swap initial mapping: [0] = 0, [1] = 1, ...
        }
    }
    else {
//    int k = 1;
//    quBitConfiguration[0] = 0;
//    for(int i = 0; k < rows && i < rows; i++)
//        for(int j = i + 1; k < rows && j < rows; j++)
//            if((quBitConfiguration[k] == -1) && ((couplingMap[i][j] == 1) || (couplingMap[i][j] == -1)))
//                quBitConfiguration[k++] = j;
    }
//    printMappings();
}

int QuCircuit::swapAlongPath(int* parent, int source, int destination)
{
    int count = 0;
    if (parent[destination] != -1){
        count = swapAlongPath(parent, source, parent[destination]) + 1;
        std::cout << "Swap: <" << parent[destination] << ", " << destination << ">" << std::endl;
        swap(logicalToPhysicalMapping[physicalToLogicalMapping[parent[destination]]], logicalToPhysicalMapping[physicalToLogicalMapping[destination]]);
        swap(physicalToLogicalMapping[parent[destination]], physicalToLogicalMapping[destination]);
        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
        int* args = swapGate -> getArgIndex();
        args[0] = source;
        args[1] = destination;
        instructionsV1.push_back(swapGate);
    }
    return count;
}

int QuCircuit::findSwapsFor1Instruction(QuGate *quGate, int **couplingMap) {
    ShortestPathFinder spf(couplingMap, rows);
    int* parent = NULL;
    int inputs = quGate -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    int physicalIndex1 = logicalToPhysicalMapping[quBitIndexes[0]];
    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
    if(inputs == 2){
        int physicalIndex2 = logicalToPhysicalMapping[quBitIndexes[1]];
        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        if(swaps == 0)
            cout << "No swap required!" << endl;
        printMappings();
    }
    instructionsV1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}

void QuCircuit::printMappings() {
    cout << "Printing physical-logical qubit mappings: " << endl;
    for(int i = 0; i < rows; i++) {
        cout << "Q" << i << " -> q" << physicalToLogicalMapping[i] << endl;
    }
    cout << endl;
//    cout << "Printing logical-physical qubit mappings: " << endl;
//    for(int i = 0; i < rows; i++) {
//        cout << "Q" << logicalToPhysicalMapping[i] << " -> q" << i << endl;
//    }
//    cout << endl;
}

int QuCircuit::findTotalSwaps(int** couplingMap) {
    int total = 0;
    for(QuGate* quGate: instructions){
        total += findSwapsFor1Instruction(quGate, couplingMap);
    }
    return total;
}

void QuCircuit::printInstructions() {
    try {
        for (QuGate *quGate: instructionsV1) {
            cout << *quGate << endl;
        }
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }
}

int QuCircuit::getRows() const {
    return rows;
}

void QuCircuit::setCols(int cols) {
    this -> cols = cols;
}

void QuCircuit::setGrid(QuGate ***grid) {
    this -> grid = grid;
}

void QuCircuit::setInstructions(const vector<QuGate*> instructions) {
    this -> instructions = instructions;
}

const vector<QuGate*> QuCircuit::getInstructionsV1() const{
    return instructionsV1;
}

const vector<QuGate*> QuCircuit::getInstructions() const{
    return instructions;
}
