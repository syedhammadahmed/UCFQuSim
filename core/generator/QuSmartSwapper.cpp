//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"
#include <bits/stdc++.h>
#include <AllShortestPathsFinder.h>
#include <util/Util.h>

vector<QuGate*> QuSmartSwapper::removeUnaryInstructions(){
    vector<QuGate*> instructions = circuit.getInstructions();
    for(QuGate* currentInstruction: instructions){
        if(currentInstruction->getCardinality()> 1) {
            nonUnaryInstructions.push_back(currentInstruction);
        }
    }
    return nonUnaryInstructions;
}

int QuSmartSwapper::findTotalSwaps(QuArchitecture& quArchitecture) {
    unsigned int total = 0;

    allSPF = new AllShortestPathsFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());

    removeUnaryInstructions();
    int t = 0;
    int lastDecreased = 0;

    for(QuGate* currentInstruction: nonUnaryInstructions){
        unsigned int min = INT32_MAX;
        int mappingCount = 0;
        vector<vector<vector<int>>> mappingWiseShortestPaths;
        vector<vector<vector<int>>> filteredMappingWiseShortestPaths;
        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();

        perInstructionMappingCounter = 0;   // needed in getCurrentMapping()

        if(Util::verbose) {
            cout << "Current Instruction: " << *currentInstruction << endl;  //todo commented to print results
            cout << "Program Counter: " << programCounter << endl;
        }

        for (QuMapping mapping: inputMappings) {   // input mappings for an instruction
            swapPath.clear();   // for 1 SP
            allSPFSwapPaths.clear();    // for all SPs
            // todo mapping never used
            // find swap count for a particular input mapping
            unsigned int swaps = findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap());
            mappingWiseShortestPaths.push_back(allSPFSwapPaths);    // all SPFs for current instruction & current mapping
            if (swaps < min)
                min = swaps;
            perInstructionMappingCounter++; // needed in getCurrentMapping()
        }
        total += min;
        if(Util::verbose)
            cout << "Min Mapping Swaps: " << min << endl; //todo commented to print results
        // remove larger swap paths and mappings
        vector<QuMapping> filteredInputMappings;

        mappingCount = 0;
        for (QuMapping mapping: inputMappings) {   // input mappings for an instruction
            if (!mappingWiseShortestPaths[mappingCount].empty() &&
                (mappingWiseShortestPaths[mappingCount][0].size() - 2 == min)) {
                filteredInputMappings.push_back(mapping);
                filteredMappingWiseShortestPaths.push_back(mappingWiseShortestPaths[mappingCount]);
            }
            mappingCount++;
        }

        vector<QuMapping> nextInstructionMappings;

        for (unsigned int j = 0; j < filteredInputMappings.size(); j++) {
            vector<QuMapping> temp;
            for (unsigned int k = 0; k < filteredMappingWiseShortestPaths[j].size(); k++) {
                temp = findAllMappingsFromPermutations(filteredInputMappings[j], filteredMappingWiseShortestPaths[j][k]);
            }
            nextInstructionMappings.insert(nextInstructionMappings.end(), temp.begin(), temp.end());
        }
        if(nextInstructionMappings.size() < t) {
//            Util::println("DECREASED!!!");
            lastDecreased = programCounter;
        }
        t = nextInstructionMappings.size();
        Util::println("nextInstructionMappings.size(): " + to_string(nextInstructionMappings.size()));
        instructionWiseMappings.push_back(nextInstructionMappings);
        programCounter++;
        circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
    }
    Util::println("LAST DECREASED: " + to_string(lastDecreased));
    delete allSPF;
    return total;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappingsFromPermutations(QuMapping& inputMapping, vector<int> swapSequence) {
    vector<QuMapping> mappings;
    unsigned int totalMoves = swapSequence.size() - 2;
    int *quBitIndexes = nonUnaryInstructions[programCounter]->getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];
    int srcMoves = 0;
    int destMoves = 0;
    if(totalMoves > 0){
        Util::println("Instruction #: " + to_string(programCounter + 1));
        for (unsigned int i = 0; i <= totalMoves; i++) {
            Util::println("Permutation #: " + to_string(i + 1));
            QuMapping mapping = inputMapping;
            vector<int> srcSeq, destSeq, tempSeq;
            srcMoves = totalMoves - i;
            destMoves = i;
            srcSeq.push_back(mapping.getPhysicalBit(src));
            for (int j = 0; j < srcMoves; j++) {
                int val = swapSequence.at(j + 1);
//                Util::println("Logical Swap: <" + to_string(src) + ", " + to_string(mapping.getLogicalMapping(val)) + ">");  // todo commented for results print.
//                Util::println("Physical Swap: <" + to_string(mapping.getPhysicalBit(src)) + ", " + to_string(val) + ">");  // todo commented for results print.
                srcSeq.push_back(val);
            }
            destSeq.push_back(mapping.getPhysicalBit(dest));
            for (int j = 0; j < destMoves; j++) {
//                Util::println("Logical Swap: <" + to_string(mapping.getLogicalMapping(swapSequence[totalMoves - j])) + ", " + to_string(dest) + ">"); // todo commented for results print.
//                Util::println("Physical Swap: <" + to_string(swapSequence[totalMoves - j]) + ", " + to_string(mapping.getPhysicalBit(dest)) + ">"); // todo commented for results print.
                destSeq.push_back(swapSequence[totalMoves - j]);
            }
//            Util::println("Before: "); // todo commented for results print.
//            mapping.print();
//            Util::println("After: ");
            mapping.fixMappings(srcSeq);
    //        for(int i=0;)
    //        reverse(destSeq.begin(), destSeq.end());
    //        Util::println("reversed : ");
//            printSwapPath(destSeq);
            mapping.fixMappings(destSeq);
//            mapping.print();
            mappings.push_back(mapping);
        }
    }
    if(mappings.empty())
        mappings.push_back(inputMapping);
    return mappings;
}

vector<QuMapping> QuSmartSwapper::getAllMappingsForCurrentInstruction() {
    vector<QuMapping> mappings;
    if (!programCounter || instructionWiseMappings.empty())  // 1st instruction
        mappings.push_back(initialMapping);  // 1st instruction has 1 default input mapping
    else {
        mappings = instructionWiseMappings[programCounter-1];
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
    int* parent = nullptr;
    int cardinality = currentInstruction -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = currentInstruction -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    vector<int> shortestPath;
    ShortestPathFinder spf(couplingMap, circuit.getRows());

    QuMapping mapping = getCurrentMapping();
    int src = mapping.getPhysicalBit(quBitIndexes[0]);
    int dest = mapping.getPhysicalBit(quBitIndexes[1]);

    //
    string temp = to_string(src) + "," + to_string(dest);
    shortestPath = preCalShortestPaths[temp];
    if(shortestPath.empty()) {
//        Util::println("Getting precalculated single SP...");
//    }
//    else {
        parent = spf.findSingleSourceShortestPaths(couplingMap, src);
        shortestPath = swapAlongPath(parent, src, parent[dest]);
        shortestPath.insert(shortestPath.begin(), src); // add src to sequence
        shortestPath.push_back(dest); // add dest to sequence
        preCalShortestPaths[temp] = shortestPath;
    }

//        mapping.print(); // todo commented for results print.
    swaps = shortestPath.size() - 2;

    allSPFSwapPaths = allSPF->findSingleSourceAllShortestPaths(src, dest, swaps); // todo find shortest w/o swaps arg
    return swaps;
}

vector<int> QuSmartSwapper::swapAlongPath(int* parent, int source, int destination) {
//    int count = 0;

    if (parent[destination] != -1){
        swapAlongPath(parent, source, parent[destination]);
        swapPath.push_back(destination);
        // insert swap gate in circuit
//        insertSwapGates(source, destination);  // todo swap gates inserted in 1 sequence fro src to dest. must be swapPath dependent
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
        : QuSwapStrategy(circuit), initialMapping(circuit.getRows()), perInstructionMappingCounter(0) {}

void QuSmartSwapper::printSwapPath(vector<int> swapPath) {
    for(int i=0; i<swapPath.size(); i++) {
        Util::print(to_string(swapPath[i]));
        if(i < swapPath.size()-1) Util::print(", ");
    }
    Util::println("");

}

