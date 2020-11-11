//
// Created by SHA on 10/22/2019.
//

#include "QuCircuitEvaluator.h"
#include "util/Util.h"
#include "../ShortestPathFinder.h"
#include "core/gates/QuGateFactory.h"
#include <iostream>

using namespace std;

QuCircuitEvaluator::QuCircuitEvaluator(QuCircuit &circuit) : circuit(circuit) {
    logicalToPhysicalMapping = new int[circuit.getRows()];
    physicalToLogicalMapping = new int[circuit.getRows()];
    initializeMappings(NULL);
}

void QuCircuitEvaluator::initializeMappings(int** couplingMap){
    if(couplingMap == NULL){
        for(int i=0; i<circuit.getRows(); i++) {
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


bool QuCircuitEvaluator::evaluateCNOTConstraints(int** couplingMap) {
    bool satisfied = true;
    int arg1 = 0;
    int arg2 = 0;
    vector<QuGate*> instructions = circuit.getInstructions();
//    cout << "Before evaluator: " << endl;
//    printMappings();
    try {
        for (QuGate *quGate: instructions) {
//            cout << endl;
//            printMappings();
//            cout << endl;
            string quGateName = Util::toLower(quGate->getMnemonic());
            arg1 = quGate -> getArgIndex()[0];
//            cout << "Evaluating instruction: " << *quGate << " : ";
            if (quGate -> getCardinality() > 1) {
                arg2 = quGate->getArgIndex()[1];
                if(quGateName == "swap") {
                    swap(logicalToPhysicalMapping[arg1],logicalToPhysicalMapping[arg2]);
                    swap(physicalToLogicalMapping[logicalToPhysicalMapping[arg1]], physicalToLogicalMapping[logicalToPhysicalMapping[arg2]]);
//                    cout << "pass: default" << endl;
                }
                else{
                    if((couplingMap[logicalToPhysicalMapping[arg1]][logicalToPhysicalMapping[arg2]] == 1)
                         || (couplingMap[logicalToPhysicalMapping[arg1]][logicalToPhysicalMapping[arg2]]) == -1){
//                        cout << "pass: qubits adjacent!" << endl;
                    // todo need to check for H gate - for the case where direction is different
                    }
                    else{
//                        cout << "fail: qubits not adjacent!" << endl;
                        satisfied = false;
//                        break;
                    }

                }
            }
            else{
//                cout << "pass : default" << endl;
            }
        }
//    cout << "After evaluator: " << endl;
//    printMappings();
    } catch (exception& e){
        cout << "Exception : " << e.what() << '\n';
    }

    if(satisfied)
        cout << "CNOT Constraints satisfied!" << endl;
    else
        cout << "CNOT Constraints NOT satisfied! Please make sure all non-unary gates have adjacent qubits!" << endl;

    return satisfied;
}
//int QuCircuitEvaluator::swapAlongPath(int* parent, int source, int destination)
//{
//    int count = 0;
//    if (parent[destination] != -1){
//        count = swapAlongPath(parent, source, parent[destination]) + 1;
//        std::cout << "Swap: <" << parent[destination] << ", " << destination << ">" << std::endl;
//    }
//    return count;
//}

//int QuCircuitEvaluator::findCostFor1Instruction(QuGate *quGate, int **couplingMap) {
//    ShortestPathFinder spf(couplingMap, circuit.getRows());
//    int* parent = NULL;
//    int inputs = quGate -> getCardinality(); // # of qubits in a gate
//    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
//    int swaps = 0;
//    int physicalIndex1 = logicalToPhysicalMapping[quBitIndexes[0]];
//    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
//    if(inputs == 2 && quGate -> getMnemonic()){
//        int physicalIndex2 = logicalToPhysicalMapping[quBitIndexes[1]];
//        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
//        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        if(swaps == 0)
//            cout << "No swap required!" << endl;
//    }
//    instructionsV1.push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
//    return swaps;
//}

void QuCircuitEvaluator::printMappings() {
    cout << "Printing physical-logical qubit mappings: " << endl;
    for(int i = 0; i < circuit.getRows(); i++) {
        cout << "Q" << i << " -> q" << physicalToLogicalMapping[i] << endl;
    }
    cout << endl;
//    cout << "Printing logical-physical qubit mappings: " << endl;
//    for(int i = 0; i < rows; i++) {
//        cout << "Q" << logicalToPhysicalMapping[i] << " -> q" << i << endl;
//    }
//    cout << endl;
}

QuCircuitEvaluator::~QuCircuitEvaluator() {
    delete [] logicalToPhysicalMapping;
    delete [] physicalToLogicalMapping;
}
