//
// Created by SHA on 9/26/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <core/generator/QuMappingInitializer.h>
#include "QuCircuit.h"
#include "QuGateFactory.h"
#include "../ShortestPathFinder.h"
#include "util/Util.h"
#include "core/generator/QuSwapStrategy.h"
#include "core/generator/QuNaiiveSwapper.h"
#include "core/generator/QuSmartSwapper.h"
#include "QuArchitecture.h"

using namespace std;

QuCircuit::QuCircuit() {
    if(Util::verbose) {
        cout << "QuCircuit default" << endl;
    }
}

QuCircuit::QuCircuit(QuArchitecture& architecture): rows(architecture.getN()), grid(NULL) {
//    cout << "QuCircuit parameterized!" << endl;
    init1();
}

//QuCircuit::QuCircuit(int rows): rows(rows), grid(NULL), mapping(rows) {
QuCircuit::QuCircuit(int rows): rows(rows), grid(NULL) {
//    cout << "QuCircuit parameterized!" << endl;
    init1();
}
//QuCircuit::QuCircuit(int rows): rows(rows), logicalToPhysicalMapping(NULL), physicalToLogicalMapping(NULL), grid(NULL) {
//    init1();
//}

//QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), grid(NULL), mapping(rows) {
QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), grid(NULL){
    init1();
}
//QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), logicalToPhysicalMapping(NULL), physicalToLogicalMapping(NULL), grid(NULL) {
//    init1();
//}

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
//    for(QuGate* ptr: instructions){
//        if(ptr != nullptr)
//            delete ptr;
//    }
//    for(int i=0; i<instructionsV1.size(); i++){
//            delete instructionsV1[i];
//    }

//    cout << "~QuCircuit()" << endl;
//    delete [] physicalToLogicalMapping;
//    delete [] logicalQuBits;
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
    if(Util::verbose)
        cout << "Circuit execution start..." << endl;
    // todo: apply each gate depth-wise
    if(Util::verbose)
        cout << "Circuit execution end..." << endl;
}

void QuCircuit::addMapping(int logicalQuBit, int physicalQuBit) {
//    mapping.setLogicalMapping(physicalQuBit, logicalQuBit);
//    logicalToPhysicalMapping[logicalQuBit] = physicalQuBit;
//    physicalToLogicalMapping[physicalQuBit] = logicalQuBit;
}

ostream &operator<<(ostream &os, const QuCircuit &circuit) {
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

                    int s = gate->getArgAtIndex(0);
                    int t = gate->getArgAtIndex(1);
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
        os << endl;
    }
    return os;
}

// initializes the 1D arrays - qubits, mappings, etc.
void QuCircuit::init1() {
//    logicalToPhysicalMapping = new int[rows];
//    physicalToLogicalMapping = new int[rows];
//    logicalQuBits = new QuBit[rows];
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

void QuCircuit::initializeMappings(QuArchitecture& quArchitecture){
//    mapping.init(0);
//    printMappings();
}

//vector<int> QuCircuit::swapAlongPath(int* parent, int source, int destination) {
//    int count = 0;
//
//    if (parent[destination] != -1){
////        count = swapAlongPath(parent, mapping.getPhysicalBit(mapping.getLogicalMapping(source)), mapping.getPhysicalBit(mapping.getLogicalMapping(parent[destination]))) + 1;
//        swapAlongPath(parent, source, parent[destination]);
////        count = swapAlongPath(parent, source, parent[destination]) + 1;
////        cout << "(" << source << " | ) " << destination << " -> ";
////        cout << "Swap: <";
////        cout << mapping.getLogicalMapping(source) << ", "
////             << mapping.getLogicalMapping(destination) << ">" << endl;
//        swapPath.push_back(destination);
////        mapping.quSwap(source, destination);
////        int temp = mapping.getLogicalMapping(parent[destination]);
////        mapping.setLogicalMapping(parent[destination], mapping.getLogicalMapping(destination));
////        mapping.setLogicalMapping(destination, temp);
//
//        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
//        int* args = swapGate -> getArgIndex();
//        args[0] = mapping.getLogicalMapping(source);
//        args[1] = mapping.getLogicalMapping(destination);
//        instructionsV1.push_back(swapGate);
//    }
//    return swapPath;
//}


//int QuCircuit::swapAlongPath(int* parent, int source, int destination)
//{
//    int count = 0;
//    if (parent[destination] != -1){
//        count = swapAlongPath(parent, source, parent[destination]) + 1;
//        cout << "Swap: <";
//        cout << physicalToLogicalMapping[source] << ", " << physicalToLogicalMapping[destination] << ">" << endl;
//        swap(physicalToLogicalMapping[parent[destination]], physicalToLogicalMapping[destination]);
//        swap(logicalToPhysicalMapping[physicalToLogicalMapping[parent[destination]]], logicalToPhysicalMapping[physicalToLogicalMapping[destination]]);
//
//        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
//        int* args = swapGate -> getArgIndex();
//        args[0] = physicalToLogicalMapping[source];
//        args[1] = physicalToLogicalMapping[destination];
//        instructionsV1.push_back(swapGate);
//    }
//    return count;
//}


void QuCircuit::findCostFor1Instruction(QuGate *quGate, int **couplingMap) {
//    QuSwapStrategy* strategy = new QuNaiiveSwapper(*this);
//    QuSwapStrategy* strategy = new QuSmartSwapper(*this);
//    int swaps = strategy->findShortestPathsFor1InputMapping(quGate, couplingMap);
//    delete strategy;
//    return swaps;
//    return 0;
}


//int QuCircuit::findShortestPathsFor1InputMapping(QuGate *quGate, int **couplingMap) {
//    ShortestPathFinder spf(couplingMap, rows);
//    int* parent = NULL;
//    int inputs = quGate -> getCardinality(); // # of qubits in a gate
//    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
//    int swaps = 0;
//    int physicalIndex1 = mapping.getPhysicalBit(quBitIndexes[0]);
//    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
//    if(inputs == 2){
//        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
////        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
//        swapPath.clear();
//        swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        mapping.fixMappings(physicalIndex1, swapPath);
////        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        swaps = swapPath.size();
////        if(swaps == 0)
////            cout << "No swap required!" << endl;
////        printMappings();
//    }
//    instructionsV1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
//    return swaps;
//}

//int QuCircuit::findShortestPathsFor1InputMapping(QuGate *quGate, int **couplingMap) {
//    ShortestPathFinder spf(couplingMap, rows);
//    int* parent = NULL;
//    int inputs = quGate -> getCardinality(); // # of qubits in a gate
//    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
//    int swaps = 0;
//    int physicalIndex1 = mapping.getPhysicalBit(quBitIndexes[0]);
//    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
//    if(inputs == 2){
//        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
//        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
//        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
////        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        if(swaps == 0)
//            cout << "No swap required!" << endl;
//        printMappings();
//    }
//    instructionsV1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
//    return swaps;
//}
//
//void QuCircuit::printMappings() {
//    cout << "Printing physical-logical qubit mappings: " << endl;
//    for(int i = 0; i < rows; i++) {
//        cout << "Q" << i << " -> q" << mapping.getLogicalMapping(i) << endl;
////        cout << "Q" << i << " -> q" << physicalToLogicalMapping[i] << endl;
//    }
//    cout << endl;
////    cout << "Printing logical-physical qubit mappings: " << endl;
////    for(int i = 0; i < rows; i++) {
////        cout << "Q" << logicalToPhysicalMapping[i] << " -> q" << i << endl;
////    }
////    cout << endl;
//}

pair<int, QuMapping> QuCircuit::findTotalSwaps(QuArchitecture& quArchitecture) {
    QuSwapStrategy *strategy = new QuSmartSwapper(*this, quArchitecture);
    cost = strategy->findTotalSwaps();
    QuMapping initialMapping = ((QuSmartSwapper*)strategy)->generateOptimalInstructions();
    delete strategy;
    return make_pair(cost, initialMapping);
}

void QuCircuit::printInstructions() {
    if(Util::verbose) {
        try {
            for (QuGate *quGate: instructionsV1) {
                cout << *quGate << endl;
            }
        } catch (exception &e) {
            cout << "Exception : " << e.what() << '\n';
        }
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

vector<QuGate*>& QuCircuit::getInstructionsV1(){
    return instructionsV1;
}

void QuCircuit::setInstructionsV1(const vector<QuGate*>& instructionsV1) {
    this->instructionsV1 = instructionsV1;
}

vector<QuGate*> QuCircuit::getInstructions() const{
    return instructions;
}

QuMapping& QuCircuit::getMapping() {
    return mapping;
}

void QuCircuit::setFileName(const string &fileName) {
    this->fileName = fileName;
}

void QuCircuit::setSimpleGrid(int** simpleGrid) {
    this->simpleGrid = simpleGrid;
}

const string &QuCircuit::getFileName() const {
    return fileName;
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

const vector<int> &QuCircuit::getSrcFrequencies() const {
    return srcFrequencies;
}

void QuCircuit::setSrcFrequencies(const vector<int> &srcFrequencies) {
    this->srcFrequencies = srcFrequencies;
}

const vector<int> &QuCircuit::getDestFrequencies() const {
    return destFrequencies;
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


