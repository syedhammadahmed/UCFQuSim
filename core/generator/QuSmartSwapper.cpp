//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"
#include <bits/stdc++.h>
#include <AllShortestPathsFinder.h>

vector<QuGate*> QuSmartSwapper::removeUnaryInstructions(){
    vector<QuGate*> instructions = circuit.getInstructions();
    for(QuGate* currentInstruction: instructions){
//        QuGate* currentInstruction = instructions[i];
        if(currentInstruction->getCardinality()> 1) {
//            cout << "Adding Instruction: " << *currentInstruction << endl;  //todo commented to print results
            nonUnaryInstructions.push_back(currentInstruction);
        }
    }
    return nonUnaryInstructions;
}

int QuSmartSwapper::findTotalSwaps(QuArchitecture& quArchitecture) {
    unsigned int total = 0;

    removeUnaryInstructions();
    for(QuGate* currentInstruction: nonUnaryInstructions){
        unsigned int min = INT32_MAX;
        int mappingCount = 0;
        perInstructionMappingCounter = 0;   // needed in getCurrentMapping()

        cout << "Current Instruction: " << *currentInstruction << endl;  //todo commented to print results
        cout << "Program Counter: " << programCounter << endl;

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();
        vector<vector<int>> paths;  // saves the swap path of each input mapping for current instruction (shortest  path)

        vector<vector<vector<int>>> mappingWiseSwapPaths;
        vector<vector<vector<int>>> filteredMappingWiseSwapPaths;

        for (QuMapping mapping: inputMappings) {   // input mappings for an instruction
            swapPath.clear();
            allSPFSwapPaths.clear();
            // todo mapping never used
            // find swap count for a particular input mapping
            unsigned int swaps = findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap());
            // allSPF modification start TODO

            mappingWiseSwapPaths.push_back(allSPFSwapPaths);
//            for(vector<int> spfPath: allSPFSwapPaths) {
//                cout << "Swap Path: ";  // todo commented for results print
//                printSwapPath(spfPath); // physical path
//                cout << "Swaps: " << swaps << endl;
//                mappingWiseSwapPaths[mappingCount].push_back(spfPath);  // add ith all SPF paths for ith mapping
////                paths.push_back(spfPath);  // add ith path for ith mapping
//            }
//            paths.push_back(swapPath);  // add ith path for ith mapping
            // allSPF modification end
            //
            if (swaps < min)
                min = swaps;
            perInstructionMappingCounter++; // needed in getCurrentMapping()
        }
        total += min;
        cout << "Min Mapping Swaps: " << min << endl; //todo commented to print results
        // remove larger swap paths and mappings
        vector<QuMapping> filteredInputMappings;
        vector<vector<int>> filteredPaths;

//        if(min > 0) {
//        if(programCounter > 0) {
        mappingCount = 0;
        for (QuMapping mapping: inputMappings) {   // input mappings for an instruction
            if (!mappingWiseSwapPaths[mappingCount].empty() &&
                (mappingWiseSwapPaths[mappingCount][0].size() - 2 == min)) {
                filteredInputMappings.push_back(mapping);
                filteredMappingWiseSwapPaths.push_back(mappingWiseSwapPaths[mappingCount]);
//                for (vector<int> mpath: mappingWiseSwapPaths[mappingCount]) {
//                    filteredPaths.push_back(mpath);
//                }
            }
            mappingCount++;
        }

        for (unsigned int j = 0; j < filteredInputMappings.size(); j++) {
            vector<QuMapping> currentInstructionMappings;
            for (unsigned int k = 0; k < filteredMappingWiseSwapPaths[j].size(); k++) {
                vector<QuMapping> temp = findAllMappingsFromPermutations(filteredInputMappings[j], filteredMappingWiseSwapPaths[j][k]);
                currentInstructionMappings.insert(currentInstructionMappings.end(), temp.begin(), temp.end());
                cout << "currentInstructionMappings.size(): " << currentInstructionMappings.size() << endl;
            }
            instructionWiseMappings.push_back(currentInstructionMappings);
        }
        programCounter++;
        circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
//        delete [] mappingWiseSwapPaths;
    }
    return total;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappingsFromPermutations(QuMapping& inputMapping, vector<int> swapSequence) {
    vector<QuMapping> mappings;
    unsigned int totalMoves = swapSequence.size() - 2;
//    int* quBitIndexes = circuit.getInstructions()[programCounter] -> getArgIndex(); // logical qubit index values
    int *quBitIndexes = nonUnaryInstructions[programCounter]->getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];
    int srcMoves = 0;
    int destMoves = 0;
    if(totalMoves > 0){
        for (unsigned int i = 0; i <= totalMoves; i++) {
            QuMapping mapping = inputMapping;
            vector<int> srcSeq, destSeq, tempSeq;
            cout << "Instruction #: " << programCounter + 1 << endl; // todo commented for results print.
            cout << "Permutation #: " << i + 1 << endl;
            srcMoves = totalMoves - i;
            destMoves = i;
//            tempSeq.push_back(mapping.getPhysicalBit(src));
            srcSeq.push_back(mapping.getPhysicalBit(src));
            for (int j = 0; j < srcMoves; j++) {
                int val = swapSequence.at(j + 1);
                cout << "Swap: <" << mapping.getPhysicalBit(src) << ", " << val << ">" << endl;  // todo commented for results print.
                if(src == val)
                    cout << "hello !!!!!!!!!!!!!!!!" << endl;
    //            cout << "Swap: <" << mapping.getLogicalMapping(src) << ", " << mapping.getLogicalMapping(val) << ">" << endl;  // todo commented for results print.
                srcSeq.push_back(val);
    //            destSeq.push_back(swapSequence[j + 1]);
//                tempSeq.push_back(val);
            }
            destSeq.push_back(mapping.getPhysicalBit(dest));
            for (int j = 0; j < destMoves; j++) {
    //        for(int j=destMoves-1; j>=0; j--){
    //            cout << "Swap: <" << mapping.getLogicalMapping(swapSequence[totalMoves-j]) << ", " << mapping.getLogicalMapping(dest) << ">" << endl; // todo commented for results print.
                cout << "Swap: <" << swapSequence[totalMoves - j] << ", " << mapping.getPhysicalBit(dest) << ">"
                     << endl; // todo commented for results print.
    //            srcSeq.push_back(swapSequence[totalMoves-j]);
    //            destSeq.push_back(dest);
                destSeq.push_back(swapSequence[totalMoves - j]);
//                tempSeq.push_back(swapSequence[totalMoves - j]);
    //            tempSeq.push_back(dest);
            }
//            tempSeq.push_back(dest);
//            tempSeq.push_back(mapping.getPhysicalBit(dest));
//            cout << "tempSeq: ";
//            printSwapPath(tempSeq);
            cout << "Before: " << endl; // todo commented for results print.
            mapping.print();
    //        mapping.fixMappings()
            //        int n = srcSeq.size();
    //        for(int i=0; i<n; i++){
    //            cout << "Swapping now ... >>> " << srcSeq[i] << " , " << destSeq[i] << endl;
    //            mapping.quSwapLogical(srcSeq[i], destSeq[i]);
    //        }
            // todo commented for results print.
            cout << "After: " << endl;
            mapping.fixMappings(srcSeq);
    //        for(int i=0;)
    //        reverse(destSeq.begin(), destSeq.end());
    //        cout << "reversed : " << endl;
//            printSwapPath(destSeq);
            mapping.fixMappings(destSeq);
            mapping.print();
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
    ShortestPathFinder spf(couplingMap, circuit.getRows());
    int* parent = nullptr;
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

        //
        cout << "ALL SPF PRINT START --- " << endl << endl;
        AllShortestPathsFinder allSPF(couplingMap, circuit.getRows());
        allSPFSwapPaths = allSPF.findSingleSourceAllShortestPaths(physicalIndex1, physicalIndex2, swaps);
//        int k=0;
//        for (vector<int> swapPath: allSPFSwapPaths){
//            cout << "Swap Path #: " << k++ << endl;
//            printSwapPath(swapPath);
//        }
        cout << "ALL SPF PRINT END --- " << endl << endl;
        //

    }
//    circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
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



