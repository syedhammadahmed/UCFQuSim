//
// Created by SHA on 9/13/20.
//

#include <core/gates/QuGate.h>
#include <iostream>
#include <util/Util.h>
#include <iomanip>
#include <algorithm>
#include "QuCircuitLayerManager.h"

QuCircuitLayerManager* QuCircuitLayerManager::instance = nullptr;

QuCircuitLayerManager::QuCircuitLayerManager(vector<QuGate*> instructions, int rows) : rows(rows), cols(instructions.size()), instructions(instructions) {
    init();

}

void QuCircuitLayerManager::buildGrid() {
    int currentInstruction = 0;
    for (auto& newGate: instructions) {
        int operands = newGate -> getCardinality();
        vector<int> args = newGate -> getArgIndex();
        layer = getLayerForNewGate(args, operands);
        addSimple(newGate, layer, newGate->getGateId());
        currentInstruction++;
    }
//    printSimpleGrid();
}

int QuCircuitLayerManager::getLayerForNewGate(vector<int> quBits, int operands) {
    int layer = 0;
    int max = quBitRecentLayer[quBits[0]];
    for(int i = 1; i < operands; i++){
        if(quBitRecentLayer[quBits[i]] > max)
            max = quBitRecentLayer[quBits[i]];
    }
    layer = max + 1;
//    while(operands>1 && somethingInBetween(quBits, operands, layer))
    while(operands>1 && somethingInBetween(quBits, operands, layer))
        layer++;
    for(int i = 0; i < operands; i++) {
        quBitRecentLayer[quBits[i]] = layer;
    }
    return layer;
}

void QuCircuitLayerManager::addSimple(QuGate *gate, int depth, int instructionNo) {
    vector<int> quBits = gate -> getArgIndex();
    int cardinality = gate -> getCardinality();
//    if (instructionNo == 169)
//        cout << "ho";
    for(int i = 0; i < cardinality; i++) {
//        cout << "i << \" \" << depth << \" \" << quBits[i] << \" \" << simpleGrid[quBits[i]][depth] << \" \" << instructionNo" << endl;
//        cout << i << " " << depth << " " << quBits[i] << " " << simpleGrid[quBits[i]][depth] << " " << instructionNo << endl;
        simpleGrid[quBits[i]][depth] = instructionNo;  // gateId
    }
}

bool QuCircuitLayerManager::somethingInBetween(vector<int> quBits, int operands, int layer) {
//    for(int i = 0; i < operands; i++)
//        for(int j = i + 1; j < operands; j++){
//            if (somethingInBetween(quBits[i], quBits[j], layer))
//                return true;
//        }
    return false;
}

bool QuCircuitLayerManager::somethingInBetween(int row1, int row2, int layer) {
    if (row1 > row2)
        swap(row1, row2);

    for (int i = row1+1; i < row2; i++)
        if (simpleGrid[i][layer] != -1)
            return true;
    return false;
}

void QuCircuitLayerManager::printSimpleGrid(){
//    Util::verbose = true;
    if(Util::verbose) {
        cout << "Printing Circuit Grid (grid of qugates positions): " << endl;
//        for (int i = 0; i < 50; i++) cout << "__";   // todo make it generic for any layers
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << std::setw(8);
            cout << "q[" << i << "]";
            for (int j = 0; j < cols; j++) {
//            for (int j = 0; j < 1; j++) {
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
//    Util::verbose = false;
}

vector<int> QuCircuitLayerManager::getNextSourceInstructionIds() {
//    printSimpleGrid();
    int sourceId = -1;
    vector<int> qubits;
    for (int i = 0; i < rows; ++i) {
//        cout << "simpleGrid[i][0] : " << simpleGrid[i][0] << endl;
        if (simpleGrid[i][0] != -1)
            qubits.push_back(simpleGrid[i][0]);
    }

    std::sort(qubits.begin(), qubits.end());
    auto qit = std::unique(qubits.begin(), qubits.begin() + qubits.size());
    qubits.resize(std::distance(qubits.begin(), qit));

//    for (auto a: qubits) {
//        cout << ">>>source instr ids:" << a << endl;
//    }
    return qubits;
}

QuCircuitLayerManager* QuCircuitLayerManager::getInstance(vector<QuGate*> instructions, int rows) {
    if (instance == nullptr) {
        instance = new QuCircuitLayerManager(instructions, rows);
    }
    return instance;
}



int QuCircuitLayerManager::getRows() const {
    return rows;
}

void QuCircuitLayerManager::setRows(int rows) {
    QuCircuitLayerManager::rows = rows;
}

const vector<QuGate*> QuCircuitLayerManager::getInstructions() const{
    return instructions;
}

void QuCircuitLayerManager::setInstructions(const vector<QuGate*> instructions) {
    this->instructions = instructions;
}


QuCircuitLayerManager::QuCircuitLayerManager() {}

void QuCircuitLayerManager::init() {
    quBitRecentLayer.clear();
    for(int i=0; i<rows; i++)
        quBitRecentLayer.push_back(-1);

    simpleGrid.clear();
    simpleGrid.resize(rows);
    for(int i = 0; i < rows; i++)
        simpleGrid[i].resize(cols, -1);

    buildGrid();
}

void QuCircuitLayerManager::removeInstruction(int id) {
    int q1 = -1, q2 = -1;
//    cout << ">>>>>>>>>>>>>>> removing " << id << ": " << endl;
    auto it = instructions.begin();
//    cout << "after removal: ";
    while(it != instructions.end()){
        if ((*it)->getGateId() == id){
            q1 = (*it)->getArgAtIndex(0);
            q2 = (*it)->getArgAtIndex(1);
            instructions.erase(it);
        }
        else{
//            cout << (*it)->getGateId() << " ";
            it++;
        }
    }
//    cout << endl;
//    updateSimpleGrid(q1, q2);
    init();
}

void QuCircuitLayerManager::deleteInstance() {
    delete instance;
    instance = nullptr;
}

vector<int> QuCircuitLayerManager::getFirstKInstructionIds(int k) {
    vector<int> firstKIds;
//    printSimpleGrid();
    int total = 0;
    for (int i = 0; i < rows && total < k; ++i) {
        for (int j = 0; j < cols && total < k; ++j) {
            if (simpleGrid[i][j] != -1 && isNewInsturction(simpleGrid[i][j], firstKIds)) {
                firstKIds.push_back(simpleGrid[i][j]);
                total++;
            }
        }
    }
    std::sort(firstKIds.begin(), firstKIds.end());
    auto qit = std::unique(firstKIds.begin(), firstKIds.begin() + firstKIds.size());
    firstKIds.resize(std::distance(firstKIds.begin(), qit));

    for (auto a: firstKIds) {
        Util::println("K source instr ids:" + to_string(a));
    }
    return firstKIds;
}

QuGate* QuCircuitLayerManager::getInstructionById(int id){
    for (QuGate* inst: instructions) {
        if (inst->getGateId() == id) {
            return inst;
        }
    }
    return nullptr;
}

bool QuCircuitLayerManager::isNewInsturction(int instructionId, vector<int> firstKIds) {
    QuGate* currentInstruction = getInstructionById(instructionId);
    for (int id: firstKIds) {
        QuGate* oldInstruction = getInstructionById(id);
//        cout << instructionId << " : " << *currentInstruction << endl;
//        cout << id << " : " << *oldInstruction << endl << endl;
        if ((*oldInstruction) == (*currentInstruction))
            return false;
    }
    return true;
}

unordered_map<int, QuGate *> QuCircuitLayerManager::getInstructionMap() {
    unordered_map<int, QuGate*> instructionMap;
    for (auto& instruction: instructions) {
        instructionMap[instruction->getGateId()] = instruction;
    }
    return instructionMap;
}

//void QuCircuitLayerManager::updateSimpleGrid(int q1, int q2) {
////    for (int i = 0; i < rows; ++i) {
////        if (simpleGrid[i][0] != id)
//    simpleGrid[q1][0] = -1;
//    simpleGrid[q2][0] = -1;
//
////    }
//
//}

//QuCircuitLayerManager *QuCircuitLayerManager::getInstance() {
//    return instance;
//}
