//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"

//
//void QuSmartSwapper::findAllMappings(int src, int dest, QuCircuit *circuit) {
//    vector<int>& swapSequence = swapPath;
////    vector<int>& swapSequence = circuit->getSwapPath();
//    // get the path
//    int totalMoves = swapSequence.size()-2;
//
////    for(int i=0; i<swapSequence.size(); i++)
////        cout << swapSequence[i] << " ";
////    cout << endl;
//
//    for(int i=0; i<totalMoves+1; i++){
//        QuMapping mapping(circuit->getMapping());  // todo save mappings for each permutation in instructionWiseMappings
//        cout << "Permutation #: " << i+1 << endl;
//        int srcMoves = totalMoves - i;
//        int destMoves = i;
//        for(int j=0; j<srcMoves; j++)
//            cout << "Swap: <" << src << ", " << swapSequence[j+1] << ">" << endl;
//        for(int j=0; j<destMoves; j++)
//            cout << "Swap: <" << swapSequence[totalMoves-j] << ", " << dest << ">" << endl;
//        if(swapSequence.size()>0)
//            swapSequence.pop_back();
//        if(swapSequence.size()>0)
//            swapSequence.erase(swapSequence.begin());
//        mapping.fixMappings(swapSequence[0], swapSequence);
//        instructionWiseMappings[programCounter].push_back(mapping); // todo need to insert for ith instruction instead of 0th
//    }
////    printMappings();
//
//}

QuMapping QuSmartSwapper::getCurrentMapping() {
    QuMapping currentMapping = initialMapping;
    if(programCounter > 0) {
        currentMapping = instructionWiseMappings[programCounter][perInstructionMappingCounter];
        // todo: get current mapping for smart technique
    }
    return currentMapping;
}

vector<int> QuSmartSwapper::swapAlongPath(int* parent, int source, int destination) {
    int count = 0;

    if (parent[destination] != -1){
//        count = swapAlongPath(parent, mapping.getPhysicalBit(mapping.getLogicalMapping(source)), mapping.getPhysicalBit(mapping.getLogicalMapping(parent[destination]))) + 1;
        swapAlongPath(parent, source, parent[destination]);
//        count = swapAlongPath(parent, source, parent[destination]) + 1;
//        cout << "(" << source << " | ) " << destination << " -> ";
//        cout << "Swap: <";
//        cout << mapping.getLogicalMapping(source) << ", "
//             << mapping.getLogicalMapping(destination) << ">" << endl;
        swapPath.push_back(destination);
//        mapping.quSwap(source, destination);
//        int temp = mapping.getLogicalMapping(parent[destination]);
//        mapping.setLogicalMapping(parent[destination], mapping.getLogicalMapping(destination));
//        mapping.setLogicalMapping(destination, temp);

        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
        int* args = swapGate -> getArgIndex();
        args[0] = getCurrentMapping().getLogicalMapping(source);
        args[1] = getCurrentMapping().getLogicalMapping(destination);
        circuit.getInstructionsV1().push_back(swapGate);
    }
    return swapPath;
}

int QuSmartSwapper::findTotalSwaps(QuArchitecture& quArchitecture) {
    int total = 0;

    for(QuGate* currentInstruction: circuit.getInstructions()){
        cout << "Instruction #: " << programCounter++ << endl;
        // get currentW mapping
        swapPath.clear();
        total += findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap());
        vector<QuMapping> instructionAllMappings = findAllMappings();

    }
    return total;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappings() {
    vector<QuMapping> mappings;
    // todo find all QuMapping objects and then filter to find minimum swap ones
    return mappings;
}

int QuSmartSwapper::findSwapsFor1Instruction(QuGate *currentInstruction, int **couplingMap) {
    ShortestPathFinder spf(couplingMap, circuit.getRows());
    int* parent = NULL;
    int cardinality = currentInstruction -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = currentInstruction -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    QuMapping& mapping = circuit.getMapping();
    vector<int>& swapSequence = swapPath;
//    vector<int>& swapSequence = circuit.getSwapPath();
    int physicalIndex1 = mapping.getPhysicalBit(quBitIndexes[0]);

    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
    if(cardinality == 2){
        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
//        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
//        swapSequence.clear();
        swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        circuit.swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        int src = physicalIndex1;
        int dest = physicalIndex2;
        swapSequence.insert(swapSequence.begin(), src); // add src to sequence
        swapSequence.push_back(dest); // add dest to sequence
        mapping.fixMappings(physicalIndex1, swapSequence);
        mapping.print();
//        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);

        swaps = swapSequence.size() - 2;
//        if(swaps == 0)
//            cout << "No swap required!" << endl;
//        printMappings();
    }
    circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}


/*

int QuSmartSwapper::findSwaps(QuGate *quGate, int **couplingMap, QuCircuit* circuit) {
    ShortestPathFinder spf(couplingMap, circuit->getRows());
    int* parent = NULL;
    int inputs = quGate -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    int physicalIndex1 = circuit->getMapping().getPhysicalBit(quBitIndexes[0]);
    vector<int>& swapSequence = circuit->getSwapPath();
    QuMapping& mapping = circuit->getMapping();

    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);

    if(inputs == 2){
        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
        swapSequence.clear();
        circuit->swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        mapping.fixMappings(physicalIndex1, swapSequence);

        for(int i=0; i<swapSequence.size(); i++)
            cout << mapping.getLogicalMapping(swapSequence[i]) << " ";
        cout << endl;

        int src = physicalIndex1;
        int dest = physicalIndex2;
        swapSequence.insert(swapSequence.begin(), src);
        swapSequence.push_back(dest);
        findAllMappings(src, dest, circuit);

        swaps = swapSequence.size() - 2;

    }
    circuit->getInstructionsV1().push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}


void QuSmartSwapper::findAllMappings(int src, int dest, QuCircuit *circuit) {
    vector<int>& swapSequence = circuit->getSwapPath();
    // get the path
    int totalMoves = swapSequence.size()-2;

//    for(int i=0; i<swapSequence.size(); i++)
//        cout << swapSequence[i] << " ";
//    cout << endl;

    for(int i=0; i<totalMoves+1; i++){
        QuMapping mapping(circuit->getMapping());  // todo save mappings for each permutation in instructionWiseMappings
        cout << "Permutation #: " << i+1 << endl;
        int srcMoves = totalMoves - i;
        int destMoves = i;
        for(int j=0; j<srcMoves; j++)
            cout << "Swap: <" << src << ", " << swapSequence[j+1] << ">" << endl;
        for(int j=0; j<destMoves; j++)
            cout << "Swap: <" << swapSequence[totalMoves-j] << ", " << dest << ">" << endl;
        if(swapSequence.size()>0)
            swapSequence.pop_back();
        if(swapSequence.size()>0)
            swapSequence.erase(swapSequence.begin());
        mapping.fixMappings(swapSequence[0], swapSequence);
        instructionWiseMappings[circuit->getCurrentInstruction()].push_back(mapping); // todo need to insert for ith instruction instead of 0th
    }
//    printMappings();

}

void QuSmartSwapper::printMappings() {
    int i = 0;
    for(vector<QuMapping> mapping: instructionWiseMappings){
        cout << "Mappings for instruction #" << i << ":" << endl;
        for(QuMapping map: mapping){
            map.printMapping();
        }
        i++;
    }

}

*/


QuSmartSwapper::QuSmartSwapper(QuCircuit &circuit) : QuSwapStrategy(circuit), initialMapping(circuit.getRows()), perInstructionMappingCounter(0) {
//    for(int i=0; i<circuit.getInstructions().size(); i++) {
//        instructionWiseMappings.push_back(*(new vector<QuMapping>));
//        cout << "instructionWiseMappings[i].size() = " << instructionWiseMappings[i].size() << endl;
//    }
}


