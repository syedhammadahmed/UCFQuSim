//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"
#include "QuMappingInitializer.h"
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
    unsigned int totalHadamards = 0;

    cout << quArchitecture;

    allSPF = new AllShortestPathsFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());

    removeUnaryInstructions();
    unsigned long long t = 0;
    int lastDecreased = 0;

    Util::println("nonUnaryInstructions size: " + to_string(nonUnaryInstructions.size()));
    for(QuGate* currentInstruction: nonUnaryInstructions){
        Util::println("INSTRUCTION ANALYSIS START: " + to_string(programCounter));
        unsigned int min = INT32_MAX;

        int mappingCount = 0;
        vector<QuMapping> nextInstructionMappings;
        vector<vector<vector<int>>> mappingWiseShortestPaths;
        vector<vector<vector<int>>> filteredMappingWiseShortestPaths;

//        if (programCounter == 4)
//            cout << "hola";
        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();

        map<string, int> generatedSwapPathsMap;

        perInstructionMappingCounter = 0;   // needed in getCurrentMapping()

        // hadamard
//        int src = inputMappings[0].getPhysicalBit(currentInstruction->getArgIndex()[0]);
//        int dest = inputMappings[0].getPhysicalBit(currentInstruction->getArgIndex()[1]);
//        hadamards += constraintNotSatisfied(src, dest, quArchitecture.getCouplingMap());
//    Util::println("constraintIssues: " + to_string(hadamards));
        // hadamard
        // if already adjacent
        if(Util::verbose) {
            cout << "Current Instruction: " << *currentInstruction << endl;  //todo commented to print results
            cout << "Program Counter: " << programCounter << "/" << nonUnaryInstructions.size()-1 << endl;
        }
        Util::println("inputMappings.size(): " + to_string(inputMappings.size()));

        //


//        for (QuMapping& mapping: inputMappings) {   // input mappings for an instruction
//        if(programCounter == 3)
//            cout << "ho";
        for(int i=0; i<inputMappings.size(); i++){
//            QuMapping& mapping = inputMappings[i];
            swapPath.clear();   // for 1 SP
            allSPFSwapPaths.clear();    // for all SPs
            // todo mapping never used
            // find swap count for a particular input mapping
            unsigned int swaps = findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap()); // acc. to current mapping
//            cout << "SWAPS:  " << swaps << endl;
            cout << "allSPFSwapPaths.size(): " << allSPFSwapPaths.size() << endl;
            mappingWiseShortestPaths.push_back(allSPFSwapPaths);    // all SPFs for current instruction & current mapping
            if (swaps < min) {
                min = swaps;
            }
            perInstructionMappingCounter++; // needed in getCurrentMapping()
        }

        total += min;

        cout << "min: " << min << endl;

//        if(programCounter == 119)
//            cout << "A ";
        Util::println("Min Mapping Swaps: " + to_string(min)); //todo commented to print results
        // remove larger swap paths and mappings
        vector<QuMapping> filteredInputMappings;

        mappingCount = 0;
//        for (QuMapping& mapping: inputMappings) {   // input mappings for an instruction
        for(int i=0; i<inputMappings.size(); i++){
//            QuMapping& mapping = inputMappings[i];
            if (!mappingWiseShortestPaths[mappingCount].empty() &&
                (mappingWiseShortestPaths[mappingCount][0].size() - 2 == min)) {
//                inputMappings[i].setMappingId(to_string(programCounter) + "." + to_string(mappingCount));
                filteredInputMappings.push_back(inputMappings[i]);
                filteredMappingWiseShortestPaths.push_back(mappingWiseShortestPaths[mappingCount]);
            }
            mappingCount++;
        }
//        for (QuMapping& mapping: inputMappings) {
//            Util::println("mapping.getMappingId(): " + mapping.getMappingId());
//        }
        cout << "filteredInputMappings.size(): " << filteredInputMappings.size() << endl;

        vector<QuMapping> temp;
        for (unsigned int j = 0; j < filteredInputMappings.size(); j++) {
            vector<QuMapping> allPermutationMappings;

            Util::println("Mapping #: " + to_string(j));
            generatedSwapPathsMap.clear();

            for (unsigned int k = 0; k < filteredMappingWiseShortestPaths[j].size(); k++) {
                //todo legit mystery?!
//                if(filteredMappingWiseShortestPaths[j][k].size() > 2) { // non-trivial path only
//                    Util::println("LEGIT");
//                }
//                else {
//                    Util::println("NOT LEGIT");
//                }
//                if(filteredMappingWiseShortestPaths[j][k].size() > 2) { // non-trivial path only
//                    Util::println("LEGIT");
                    string pathString = Util::pathToString(filteredMappingWiseShortestPaths[j][k]);
                    int val = generatedSwapPathsMap[pathString];
                    if (val == 0) {
                        if(filteredMappingWiseShortestPaths[j][k].size() == 2) { // no swap required
//                            cout << "filteredMappingWiseShortestPaths[j][k].size == 2" << endl;
                            Util::printPath(filteredMappingWiseShortestPaths[j][k]);
                        }
                        allPermutationMappings = findAllMappingsFromPermutations(filteredInputMappings[j],
                                                               filteredMappingWiseShortestPaths[j][k],
                                                               quArchitecture);
                        temp.insert(temp.end(), allPermutationMappings.begin(), allPermutationMappings.end());
//                        generatedSwapPathsMap[pathString]++;
                        generatedSwapPathsMap.insert(pair<string,int>(pathString,1));
                        Util::println("generatedSwapPathsMap[" + pathString + "] : " +
                                      to_string(generatedSwapPathsMap[pathString]));
                    } else {
                        Util::println("Swap Path already generated earlier...");
                    }
//                }
//                else {
//                    Util::println("NOT LEGIT");
//                } x

            }
        }
        Util::println("# of mappings from permutations: "  + to_string(temp.size()));
        for(int i=0; i<temp.size(); i++){
//                            QuMapping& m = temp[i];
            if(programCounter != 0) {
                temp[i].setParentMappingId(temp[i].getMappingId());
                temp[i].setMappingId(to_string(programCounter) + "." + to_string(i));
            }
//            if(programCounter == 0)
//                temp[i].setParentMappingId("*");
            Util::println("m.getMappingId() :" + temp[i].getMappingId());
            Util::println("m.getParentMappingId() :" + temp[i].getParentMappingId());
        }

        if(!temp.empty())
            nextInstructionMappings.insert(nextInstructionMappings.end(), temp.begin(), temp.end());

        // todo generate final swap instructions to add to final program
        if(nextInstructionMappings.size() < t) {
//            Util::println("DECREASED!!!");
            lastDecreased = programCounter;
        }
        t = nextInstructionMappings.size();
//        Util::println("nextInstructionMappings.size(): " + to_string(nextInstructionMappings.size()));
        int threshold = 1000;
        if(nextInstructionMappings.size()>threshold) {
            nextInstructionMappings.erase(nextInstructionMappings.begin() + threshold, nextInstructionMappings.end());
        }
//        Util::println("nextInstructionMappings: " + to_string(nextInstructionMappings.size()));
        if(!nextInstructionMappings.empty()) {
//            cout << "nextInstructionMappings.size(): " << nextInstructionMappings.size() << endl;
//            cout << "Program Counter: " << programCounter << " - " << "nextInstructionMappings.empty()" << endl;
            instructionWiseMappings.push_back(nextInstructionMappings);
        }
//        circuit.getInstructionsV1().push_back(currentInstruction); // new program which includes swap gates for CNOT-constraint satisfaction
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
    }
    Util::println("LAST DECREASED: " + to_string(lastDecreased));

    vector<QuGate*> finalProgram;

    generateOptimalInstructions(finalProgram, quArchitecture);

//    Util::println("Hadamard Required: " + to_string(hadamards));
//    cout <<"Hadamards Required: " + to_string(hadamards) << endl;
    //
    delete allSPF;
    return total;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappingsFromPermutations(QuMapping& inputMapping, vector<int> swapSequence, QuArchitecture& quArchitecture) {
    vector<QuMapping> mappings;
    unsigned int totalMoves = swapSequence.size() - 2;
    Util::print("Swap Sequence: ");
    Util::printPath(swapSequence);
    vector<int> quBitIndexes = nonUnaryInstructions[programCounter]->getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];
    int srcMoves = 0;
    int destMoves = 0;
    Util::println("Program Counter: " + to_string(programCounter));
    if(Util::verbose) cout << "Current Instruction: " << (*nonUnaryInstructions[programCounter]) << endl;
//    if(quArchitecture.isAdjacent(inputMapping.getPhysicalBit(src), inputMapping.getPhysicalBit(dest)))
//        return instructionWiseMappings[programCounter-1];
//    if(totalMoves > 0 && !quArchitecture.isAdjacent(inputMapping.getPhysicalBit(src), inputMapping.getPhysicalBit(dest))){
    if(totalMoves > 0){
        for (unsigned int i = 0; i <= totalMoves; i++) {
//            Util::println("Permutation #: " + to_string(i));
            QuMapping mapping = inputMapping;
            vector<Swap> swapGates;
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
            vector<Swap> swapGatesTemp;
            if (srcSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(srcSeq);
                swapGates.insert(swapGates.begin(), swapGatesTemp.begin(), swapGatesTemp.end());
            }
    //        for(int i=0;)
    //        reverse(destSeq.begin(), destSeq.end());
//            Util::println("srcSeq + destSeq : START");
//            printSwapPath(srcSeq);
//            Util::println("srcSeq + destSeq : MID ");
//            printSwapPath(destSeq);
//            Util::println("srcSeq + destSeq : END ");
//
            if (destSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(destSeq);
                swapGates.insert(swapGates.end(), swapGatesTemp.begin(), swapGatesTemp.end());
            }
////            mapping.print();
////            mapping.setParentMappingId(parentId);
//            // todo add swap instructions to mapping
            if(swapGates.size()>0) {
                mapping.setSwapInstructions(swapGates);
                mappings.push_back(mapping);
            }
//            tempSeq.insert(tempSeq.end(),srcSeq.begin(), srcSeq.end());
//            tempSeq.insert(tempSeq.end(),destSeq.begin(), destSeq.end());
//            Util::print("srcSeq: ");
//            Util::printPath(srcSeq);
//            Util::print("destSeq: ");
//            Util::printPath(destSeq);

        }
    }
    if(mappings.empty()) { // no swap happened so copy it after removing swaps
        QuMapping mapping = inputMapping;
        mapping.clearSwapInstructions();
        mappings.push_back(mapping);
    }
//    cout << endl << endl;
    return mappings;
}

vector<pair<int, int>> QuSmartSwapper::makeRestrictionPairList(int k){ // top k unique instructions
    vector<pair<int, int>> restrictedPairs;
    int j = 0;
    bool dup = false;
    for (int i = 0; i < k; ++i) {
        dup = false;
        QuGate* gate = nonUnaryInstructions[j++];
        pair<int, int> newPair = make_pair(gate->getArgAtIndex(0), gate->getArgAtIndex(1));
        for (int l = 0; l < restrictedPairs.size(); ++l) {
            if (newPair == restrictedPairs[l]) {
                --i;
                dup = true;
                break;
            }
        }
        if (!dup)
            restrictedPairs.push_back(newPair);

    }
    return restrictedPairs;
}

vector<QuMapping> QuSmartSwapper::generateInitialMappings() { // todo add 100 mappings for instr 0
//    mappingInitializer.initGenerator(architecture.getN());
    vector<pair<int, int>> restrictionList = makeRestrictionPairList(3);
    initialMappings = mappingInitializer.generateSmartMappings(restrictionList, architecture);
    return initialMappings;
}

vector<QuMapping> QuSmartSwapper::getAllMappingsForCurrentInstruction() {
    vector<QuMapping> mappings;
    if (!programCounter) {  // 1st instruction
        mappings = generateInitialMappings();
//        instructionWiseMappings.push_back(mappings);
//        initialMapping.setParentMappingId("*");
//        mappings.push_back(initialMapping);  // 1st instruction has 1 default input mapping
    } else {
        mappings = instructionWiseMappings[programCounter-1];
    }
    return mappings;
}

QuMapping QuSmartSwapper::getCurrentMapping() {
    QuMapping currentMapping;
    if (programCounter == 3)
        cout <<"ho";
    if(!programCounter)
        currentMapping = initialMappings[perInstructionMappingCounter];
    else
        currentMapping = instructionWiseMappings[programCounter-1][perInstructionMappingCounter];
//        currentMapping.setMappingId(to_string(programCounter) + "." + to_string(perInstructionMappingCounter));
//    }
    return currentMapping;
//    return instructionWiseMappings[programCounter-1][perInstructionMappingCounter];
}

int QuSmartSwapper::findSwapsFor1Instruction(QuGate *currentInstruction, int **couplingMap) {
    int* parent = nullptr;
    int cardinality = currentInstruction -> getCardinality(); // # of qubits in a gate
    vector<int> quBitIndexes = currentInstruction -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    vector<int> shortestPath;
    ShortestPathFinder spf(couplingMap, circuit.getRows());

    QuMapping mapping = getCurrentMapping();
    cout << "programCounter|perInstructionMappingCounter|mapping.isLegit(): " << programCounter<< "|" << perInstructionMappingCounter << "|" << mapping.isLegit() << endl;

    Util::println("quBitIndexes[0]: " + to_string(quBitIndexes[0]));
    Util::println("quBitIndexes[1]: " + to_string(quBitIndexes[1]));
    int src = mapping.getPhysicalBit(quBitIndexes[0]);
    int dest = mapping.getPhysicalBit(quBitIndexes[1]);
    if (src == -1 || dest == -1)
        cout << "ho";

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
        Util::println("[QuSmartSwapper::findSwapsFor1Instruction:253] shortestPath b/w " + temp + ": " + Util::pathToString(shortestPath));
    }

    Util::println("shortestPath.size(): " + to_string(shortestPath.size()));
//        mapping.print(); // todo commented for results print.
    swaps = shortestPath.size() - 2;
    cout << "src||dest||swaps: " << src << "||" << dest << "||" <<  swaps << endl;
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
    vector<int> args = swapGate -> getArgIndex();
    swapGate -> setArgAtIndex(0, getCurrentMapping().getLogicalMapping(source));       // set swap gate 1st arg
    swapGate -> setArgAtIndex(1, getCurrentMapping().getLogicalMapping(destination));       // set swap gate 2nd arg
    circuit.getInstructionsV1().push_back(swapGate);               // add swap gate to circuit
}

QuSmartSwapper::QuSmartSwapper(QuCircuit &circuit, QuArchitecture& architecture)
        : QuSwapStrategy(circuit, architecture), initialMapping(circuit.getRows()), perInstructionMappingCounter(0), mappingInitializer(architecture.getN()) {}

//void QuSmartSwapper::setInitialMapping(){
//    initialMapping = mappingInitializer.getNextMapping();
//}

void QuSmartSwapper::printSwapPath(vector<int> swapPath) {
    for(int i=0; i<swapPath.size(); i++) {
        Util::print(to_string(swapPath[i]));
        if(i < swapPath.size()-1) Util::print(", ");
    }
    Util::println("");

}

unsigned int QuSmartSwapper::constraintNotSatisfied(int src, int dest, int **couplingMap) {
//    Util::println("src: " + to_string(src));
//    Util::println("dest: " + to_string(dest));
//    Util::print("couplingMap[src][dest]: ");
//    cout << couplingMap[src][dest] << endl;
    if(couplingMap[src][dest] == -1)
        return 1;
    return 0;
}

void QuSmartSwapper::generateOptimalInstructions(vector<QuGate*>& finalProgram, QuArchitecture& quArchitecture) {
    int nonUnarySize = instructionWiseMappings.size()-1;
    string parentMappingId;
    int parentProgramCounter, parentMappingCounter;
    hadamards = 0;
//    for (int k = 0; k <instructionWiseMappings.size() ; ++k) {
//        cout << "instructionWiseMappings[k][0].getMappingId(): " << instructionWiseMappings[k][0].getMappingId() << endl;
//        cout << "instructionWiseMappings[k][0].getParentMappingId(): " << instructionWiseMappings[k][0].getParentMappingId() << endl;
//
//    }
//    circuit.setInstructionsV1(circuit.getInstructions());
//    Util::println("circuit.getInstructions().size() : " + to_string(circuit.getInstructions().size()));
//    Util::println("circuit.getInstructionsV1().size() : " + to_string(circuit.getInstructionsV1().size()));

    cout << "instructionWiseMappings.size() :" << instructionWiseMappings.size() << endl;
    cout << "nonUnaryInstructions.size() :" << nonUnaryInstructions.size() << endl;

//    for(QuMapping m: mappings){
//        cout << "m.getSwapInstructions().size(): " << m.getSwapInstructions().size() << endl;
//        for(QuGate* g: m.getSwapInstructions()){
//            cout << (*g) << endl;
//        }
//    }
    QuMapping theMapping = instructionWiseMappings[nonUnarySize][0]; // get any (1st) last mapping to start backtracking
    vector<QuMapping> selectedMappings;
    Util::println("theMapping.getMappingId(): " + theMapping.getMappingId());
    Util::println("theMapping.getParentMappingId(): " + theMapping.getParentMappingId());

    int i;
    for(i = nonUnarySize-1; i>=0; i--){
        selectedMappings.insert(selectedMappings.begin(), theMapping);
        parentMappingId = theMapping.getParentMappingId();
        Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
//        Util::println("parentProgramCounter: " + to_string(parentProgramCounter));
//        Util::println("parentMappingCounter: " + to_string(parentMappingCounter));
//        vector<QuGate*> swapInstructions = theMapping.getSwapInstructions();
//        for(int i=0; i<swapInstructions.size(); i++) {
//            cout << *swapInstructions[i] << endl;
//        }
//        theMapping.print();
//        cout << *nonUnaryInstructions[i+1] << endl;
//        cout << endl;
        theMapping = instructionWiseMappings[parentProgramCounter][parentMappingCounter];
        Util::println("theMapping.getMappingId(): " + theMapping.getMappingId());
        Util::println("theMapping.getParentMappingId(): " + theMapping.getParentMappingId());
    }
    selectedMappings.insert(selectedMappings.begin(), theMapping);
//    parentMappingId = theMapping.getParentMappingId();
//    Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
//    Util::println("parentProgramCounter: " + to_string(parentProgramCounter));
//    Util::println("parentMappingCounter: " + to_string(parentMappingCounter));
//    vector<QuGate*> swapInstructions = theMapping.getSwapInstructions();
//    for(int i=0; i<swapInstructions.size(); i++) {
//        cout << *swapInstructions[i] << endl;
//    }
//    theMapping.print();
//    cout << *nonUnaryInstructions[i+1] << endl;
//    cout << endl;

//    cout << "selectedMappings.size() :" << selectedMappings.size() << endl;
//    cout << "nonUnaryInstructions.size() :" << nonUnaryInstructions.size() << endl;

    int x = 0; // x : index of next non-Unary instruction
    for(i=0; i<selectedMappings.size(); i++) {
        selectedMappings[i].print();
        Util::println("selectedMappings[i].getMappingId(): " + selectedMappings[i].getMappingId());
        Util::println("selectedMappings[i].getParentMappingId(): " + selectedMappings[i].getParentMappingId());

        x = insertRemovedUnaryInstructions(finalProgram, x);

        vector<QuGate*> swapInstructions;// = selectedMappings[i].getSwapInstructions();
        for(int j=0; j<selectedMappings[i].getSwapInstructions().size(); j++) {
            QuGate *swapGate = QuGateFactory::getQuGate("SWAP");
            swapGate->setArgAtIndex(0, selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0]);
            swapGate->setArgAtIndex(1, selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1]);
            swapInstructions.push_back(swapGate);
        }
        for(int i=0; i<swapInstructions.size(); i++) {
//            cout << *swapInstructions[i] << endl;
            finalProgram.push_back(swapInstructions[i]);
        }
//        cout << *nonUnaryInstructions[i] << endl;
//        finalProgram.push_back(nonUnaryInstructions[i]);

        hadamardCheck(finalProgram, quArchitecture, selectedMappings[i], i);
        circuit.setHadamards(hadamards);
//        cout << endl;
    }

    Util::println("selectedMappings.size(): " + to_string(selectedMappings.size()));
    Util::println("nonUnaryInstructions.size(): " + to_string(nonUnaryInstructions.size()));

    insertEndingUnaryInstructions(finalProgram);

    circuit.setInstructionsV1(finalProgram);
    cout << circuit.getFileName() << " - original circuit size: " << circuit.getInstructions().size() << endl;
    cout << circuit.getFileName() << " - hadamards: " << hadamards << endl;
    cout<< circuit.getFileName() << " - final circuit size: " << circuit.getInstructionsV1().size() << endl;
//    cout << *nonUnaryInstructions[i] << endl;

//    circuit.getInstructionsV1().push_back(currentInstruction);

}

void QuSmartSwapper::insertEndingUnaryInstructions(vector<QuGate*>& finalProgram) {
    vector<QuGate*> originalProgram = circuit.getInstructions();
    int i=originalProgram.size()-1;
    while(originalProgram[i--]->isUnary());
    finalProgram.insert(finalProgram.end(), originalProgram.begin()+i+2, originalProgram.end());
}

int QuSmartSwapper::insertRemovedUnaryInstructions(vector<QuGate*>& finalProgram, int nextNonUnaryIndex) {
    vector<QuGate*> originalProgram = circuit.getInstructions();
    int i = 0;
    int nonUnaryCounter = 0;
    originalProgram.erase(originalProgram.begin(), originalProgram.begin() + nextNonUnaryIndex);
    while(originalProgram[i]->isUnary()){
        finalProgram.push_back(originalProgram[i]);
        i++;
    }
    Util::println("nextNonUnaryIndex + i + 1: " + to_string(nextNonUnaryIndex + i + 1));
    return nextNonUnaryIndex + i + 1;

}

void
QuSmartSwapper::hadamardCheck(vector<QuGate *> &finalProgram, QuArchitecture &quArchitecture, QuMapping &currentMapping,
                              int index) {
    QuGate* currentInstruction = nonUnaryInstructions[index];
    int src = currentInstruction->getArgAtIndex(0);
    int target = currentInstruction->getArgAtIndex(1);
    int physicalSrc = currentMapping.getPhysicalBit(src);
    int physicalTarget = currentMapping.getPhysicalBit(target);
    QuGate* hadamard = NULL;
    if(!quArchitecture.isCompatable(physicalSrc, physicalTarget)){
        hadamard = QuGateFactory::getQuGate("h");
        hadamard->setArgAtIndex(0, src);
        finalProgram.push_back(hadamard);
        hadamard = QuGateFactory::getQuGate("h");
        hadamard->setArgAtIndex(0, target);
        finalProgram.push_back(hadamard);
        hadamards += 2;
    }
//    cout << *nonUnaryInstructions[index] << endl;
    finalProgram.push_back(nonUnaryInstructions[index]);
    if(!quArchitecture.isCompatable(physicalSrc, physicalTarget)){
        hadamard = QuGateFactory::getQuGate("h");
        hadamard->setArgAtIndex(0, src);
        finalProgram.push_back(hadamard);
        hadamard = QuGateFactory::getQuGate("h");
        hadamard->setArgAtIndex(0, target);
        finalProgram.push_back(hadamard);
        hadamards += 2;
    }
//    cout << "Hadamards | " << index << " : " << hadamards << endl;
}

unsigned int QuSmartSwapper::getHadamards() const {
    return hadamards;
}

