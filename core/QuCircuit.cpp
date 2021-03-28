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
    if(Util::verbose) {
        cout << "QuCircuit default" << endl;
    }
}

QuCircuit::QuCircuit(int rows): rows(rows), grid(NULL) {
    init1();
}

QuCircuit::QuCircuit(int rows, int cols): rows(rows), cols(cols), grid(NULL){
    init1();
}

QuCircuit::QuCircuit(QuArchitecture& architecture): rows(architecture.getN()), grid(NULL) {
    init1();
}

QuCircuit::~QuCircuit() {

}


//void QuCircuit::add(shared_ptr<QuGate> gate, int row, int depth) {
//    grid[row][depth] = gate;
//}

//void QuCircuit::add(shared_ptr<QuGate> gate, int depth) {
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

ostream &operator<<(ostream &os, const QuCircuit &circuit) {
    int temp = 0;
    for (int i = 0; i < circuit.rows; i++) {
//        temp = 0;
        os << "q" << i << " : ";
        for (int j = 0; j < circuit.cols; j++) {
            shared_ptr<QuGate> gate = circuit.grid[i][j];
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
//        grid[i] = new shared_ptr<QuGate>[cols];
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



void QuCircuit::findCostFor1Instruction(shared_ptr<QuGate> quGate, int **couplingMap) {
//    QuSwapStrategy* strategy = new QuNaiiveSwapper(*this);
//    QuSwapStrategy* strategy = new QuSmartSwapper(*this);
//    int swaps = strategy->findShortestPathsFor1InputMapping(quGate, couplingMap);
//    delete strategy;
//    return swaps;
//    return 0;
}


//int QuCircuit::findShortestPathsFor1InputMapping(shared_ptr<QuGate> quGate, int **couplingMap) {
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
//    instructions1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
//    return swaps;
//}

//int QuCircuit::findShortestPathsFor1InputMapping(shared_ptr<QuGate> quGate, int **couplingMap) {
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
//    instructions1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
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

//    int min = INT32_MAX;
//    int minPerm = INT32_MAX;
//    strategy->initInitializerMappingCounter();
//    for (int i = 0; i < 2 ; ++i) {
        cost = strategy->findTotalSwaps();
//        if (cost < min) {
//            min = cost;
//            minPerm = i;
//        }
//        cout << "cost : " << cost << ", min perm : " << minPerm << endl;
//
//    }
//    QuMapping initialMapping(quArchitecture.getN());
    QuMapping initialMapping = ((QuSmartSwapper*)strategy)->generateOptimalInstructions();
    delete strategy;

    return make_pair(cost, initialMapping);
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

int QuCircuit::getRows() const {
    return rows;
}

void QuCircuit::setCols(int cols) {
    this -> cols = cols;
}

void QuCircuit::setGrid(shared_ptr<QuGate>** grid) {
    this -> grid = std::move(grid);
}

void QuCircuit::setInstructions(const vector<shared_ptr<QuGate>> instructions) {
    this -> instructions0 = instructions;
}

vector<shared_ptr<QuGate>>& QuCircuit::getInstructions1(){
    return instructions1;
}

void QuCircuit::setInstructions1(const vector<shared_ptr<QuGate>>& instructions1) {
    this->instructions1 = instructions1;
}

vector<shared_ptr<QuGate>> QuCircuit::getInstructions() const{
    return instructions0;
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



