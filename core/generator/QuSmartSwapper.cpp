//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"

vector<QuGate*> QuSmartSwapper::removeUnaryInstructions(){
    vector<QuGate*> instructions = circuit.getInstructions();
    for(int i=0; i<instructions.size(); i++){
        QuGate* currentInstruction = instructions[i];
        if(currentInstruction->getCardinality()> 1) {
//            cout << "Adding Instruction: " << *currentInstruction << endl;  //todo commented to print results
            nonUnaryInstructions.push_back(currentInstruction);
        }
    }
    return nonUnaryInstructions;
}

int QuSmartSwapper::findTotalSwaps(QuArchitecture& quArchitecture) {
    int total = 0;
//    vector<QuGate*> instructions = circuit.getInstructions();
    removeUnaryInstructions();
    for(int i=0; i<nonUnaryInstructions.size(); i++){
        QuGate* currentInstruction = nonUnaryInstructions[i];
//        cout << "Current Instruction: " << *currentInstruction << endl;  //todo commented to print results

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();
        //        cout << "Instruction #: " << programCounter << endl;
        perInstructionMappingCounter = 0;   // needed in getCurrentMapping()
        unsigned int min = INT32_MAX;
        vector<vector<int>> paths;  // saves the swap path of each input mapping for current instruction (shortest  path)
        for (QuMapping mapping: inputMappings) {   // input mappings for an instruction
            swapPath.clear();
            // find swap count for a particular input mapping
            int swaps = findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap());
                cout << "Swap Path: ";  // todo commented for results print.
                printSwapPath(swapPath);
                cout << "Swaps: " << swaps << endl;

            paths.push_back(swapPath);  // add ith path for ith mapping
            if (swaps < min)
                min = swaps;
            perInstructionMappingCounter++; // needed in getCurrentMapping()
        }
        total += min;
//            cout << "Min Mapping Swaps: " << min << endl; //todo commented to print results
        // remove larger swap paths and mappingsD
        vector<QuMapping> filteredInputMappings;
        vector<vector<int>> filteredPaths;
        int n = paths.size();
        // keeping only minimum path mappings; calling them as filtered
        for (int j = 0; j < n; j++) {
            if (!paths.empty() && (paths[j].size() - 2 == min)) {
//                    cout << "Adding Mapping Having Swaps: " << paths[j].size() - 2 << endl;  //todo commented to print results
                filteredPaths.push_back(paths[j]);
                filteredInputMappings.push_back(inputMappings[j]);
                //                paths.erase(paths.begin() + j);
                //                inputMappings.erase(inputMappings.begin() + j);
            }

        }
        // find permutations of each mapping and generate mappings for next instruction
        inputMappings = filteredInputMappings;  // todo: save these to backtrack and generate o/p file
        paths = filteredPaths;                  // todo: save these to backtrack and generate o/p file
        for (int j = 0; j < inputMappings.size(); j++) {
            vector<QuMapping> currentInstructionMappings = findAllMappingsFromPermutations(inputMappings[j],
                                                                                           paths[j]);
            instructionWiseMappings.push_back(currentInstructionMappings);
            //            cout << "FilteredprintSwapPath(paths[j]);
        }
        //        for() {
        //            insertSwapGates(source, destination);
        //        }
        circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
    }
    return total;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappingsFromPermutations(QuMapping& inputMapping, vector<int> swapSequence) {
    vector<QuMapping> mappings;
    int totalMoves = swapSequence.size()-2;
//    int* quBitIndexes = circuit.getInstructions()[programCounter] -> getArgIndex(); // logical qubit index values
    int* quBitIndexes = nonUnaryInstructions[programCounter] -> getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];

    for(int i=0; i<totalMoves+1; i++){
        QuMapping mapping = inputMapping;
        vector<int> srcSeq, destSeq;
//        cout << "Instruction #: " << programCounter+1 << endl; // todo commented for results print.
//        cout << "Permutation #: " << i+1 << endl;
        int srcMoves = totalMoves - i;
        int destMoves = i;
        for(int j=0; j<srcMoves; j++) {
//            cout << "Swap: <" << src << ", " << swapSequence[j + 1] << ">" << endl;  // todo commented for results print.
            srcSeq.push_back(src);
            destSeq.push_back(swapSequence[j + 1]);
        }
        for(int j=0; j<destMoves; j++){
//            cout << "Swap: <" << swapSequence[totalMoves-j] << ", " << dest << ">" << endl; // todo commented for results print.
            srcSeq.push_back(swapSequence[totalMoves-j]);
            destSeq.push_back(dest);
        }
//        cout << "Before: " << endl; // todo commented for results print.
//        mapping.print();
        int n = srcSeq.size();
        for(int i=0; i<n; i++){
//            cout << ">>> " << srcSeq[i] << " , " << destSeq[i] << endl;
            mapping.quSwapLogical(srcSeq[i], destSeq[i]);
        }
        // todo commented for results print.
//        cout << "After: " << endl;
//        mapping.print();
        mappings.push_back(mapping);
    }
    return mappings;
}

vector<QuMapping> QuSmartSwapper::getAllMappingsForCurrentInstruction() {
    vector<QuMapping> mappings;
    if (!programCounter || instructionWiseMappings.empty())  // 1st instruction
        mappings.push_back(initialMapping);  // 1st instruction has 1 default input mapping
    else {
        mappings = instructionWiseMappings[programCounter-1];  // todo replicate mappings for 1-card instr
    }
    return mappings;
}

QuMapping QuSmartSwapper::getCurrentMapping() {
    QuMapping currentMapping = initialMapping;
    if(programCounter > 0 && !instructionWiseMappings.empty()) {
        currentMapping = instructionWiseMappings[programCounter-1][perInstructionMappingCounter];
    }
    return currentMapping;
}

int QuSmartSwapper::findSwapsFor1Instruction(QuGate *currentInstruction, int **couplingMap) {
    ShortestPathFinder spf(couplingMap, circuit.getRows());
    int* parent = NULL;
    int cardinality = currentInstruction -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = currentInstruction -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    QuMapping mapping = getCurrentMapping();
    int physicalIndex1 = mapping.getPhysicalBit(quBitIndexes[0]);

    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
    if(cardinality == 2){
        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
        swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        int src = physicalIndex1;
        int dest = physicalIndex2;
        swapPath.insert(swapPath.begin(), src); // add src to sequence
        swapPath.push_back(dest); // add dest to sequence
//        mapping.print(); // todo commented for results print.
        swaps = swapPath.size() - 2;
    }
//    circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}

vector<int> QuSmartSwapper::swapAlongPath(int* parent, int source, int destination) {
    int count = 0;

    if (parent[destination] != -1){
        swapAlongPath(parent, source, parent[destination]);
        swapPath.push_back(destination);
        // insert swap gate in circuit
//        insertSwapGates(source, destination);
//        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");    // create a new swap gate
//        int* args = swapGate -> getArgIndex();
//        args[0] = getCurrentMapping().getLogicalMapping(source);       // set swap gate 1st arg
//        args[1] = getCurrentMapping().getLogicalMapping(destination);  // set swap gate 2nd arg
//
//        circuit.getInstructionsV1().push_back(swapGate);               // add swap gate to circuit
    }
    return swapPath;
}

void QuSmartSwapper::insertSwapGates(int source, int destination){
        // insert swap gate in circuit
        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");    // create a new swap gate
        int* args = swapGate -> getArgIndex();
        args[0] = getCurrentMapping().getLogicalMapping(source);       // set swap gate 1st arg
        args[1] = getCurrentMapping().getLogicalMapping(destination);  // set swap gate 2nd arg
        circuit.getInstructionsV1().push_back(swapGate);               // add swap gate to circuit
}

QuSmartSwapper::QuSmartSwapper(QuCircuit &circuit)
        : QuSwapStrategy(circuit), initialMapping(circuit.getRows()), perInstructionMappingCounter(0) {
//    cout << "Hello " << endl;
//    cout << "QuSmartSwapper ok!" << endl;

}

void QuSmartSwapper::printSwapPath(vector<int> swapPath) {
    for(int i=0; i<swapPath.size(); i++) {
        cout << swapPath[i];
        if(i < swapPath.size()-1) cout << ", ";
    }
    cout << endl;

}
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



