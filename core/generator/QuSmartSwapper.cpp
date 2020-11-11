//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"
#include "QuGateFactory.h"
#include "QuMappingInitializer.h"
#include "QuCircuitLayerManager.h"
#include <bits/stdc++.h>
#include <AllShortestPathsFinder.h>
#include <util/Util.h>
#include <AllPairShortestPathFinder.h>

vector<QuGate*> QuSmartSwapper::removeUnaryInstructions(){
    vector<QuGate*> instructions = circuit.getInstructions();
    for(QuGate* currentInstruction: instructions){
        if(currentInstruction->getCardinality()> 1) {
            nonUnaryInstructions.push_back(currentInstruction);
        }
    }
    return nonUnaryInstructions;
}

int QuSmartSwapper::findTotalSwapsDAG(QuArchitecture& quArchitecture) {
    unsigned int totalCost = 0;
    swaps = 0;
    totalSwaps = 0;
    totalHadamards = 0;
    allSPF = new AllShortestPathsFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());
    allPairShortestPathFinder = new AllPairShortestPathFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());

    removeUnaryInstructions();
    unsigned long long t = 0;
    int lastDecreased = 0;

    Util::println("nonUnaryInstructions size: " + to_string(nonUnaryInstructions.size()));

    int totalInstructions = nonUnaryInstructions.size();
    QuGate* previousInstruction = nullptr;
    for(int i=0; i<totalInstructions; i++){
        currentInstruction = nullptr;
        Util::println("INSTRUCTION ANALYSIS START: " + to_string(programCounter));
        unsigned int minId = -1;

        currentInstructionIds = getCurrentInstructionIds(); // source instructions (independent)

        unsigned int minCostLayer = INT32_MAX;
        unsigned int minSwapLayer = INT32_MAX;
        unsigned int minHadamardLayer = INT32_MAX;
        vector<vector<vector<int>>> minMappingWiseShortestPathsLayer;
        vector<vector<int>> minMappingWiseShortestPathCostsLayer;

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();  // todo get mappings only once esp initial not re-generate

        for (auto id: currentInstructionIds) {
            perInstructionMappingCounter = 0;   // needed in getCurrentMapping()

            cout << "\n\n\nCurrent Instruction under review: " << id << endl;
            for (QuGate *inst: nonUnaryInstructions) { // todo make it efficient by using hashmap
                if (inst->getGateId() == id) {
                    currentInstruction = inst;
                    break;
                }
            }

            Util::setVerbose();
            if (Util::verbose) {
                cout << "Current Instruction: " << *currentInstruction << endl;  //todo commented to print results
                cout << "Program Counter: " << programCounter << "/" << nonUnaryInstructions.size() - 1 << endl;
            }
            Util::println("inputMappings.size(): " + to_string(inputMappings.size()));
            Util::resetVerbose();

            mappingWiseShortestPathCosts.clear();
            mappingWiseShortestPathCosts.resize(inputMappings.size());
            mappingWiseShortestPaths.clear();

            unsigned int minCostInstruction = INT32_MAX;
            unsigned int minSwapInstruction = INT32_MAX;
            unsigned int minHadamardInstruction = INT32_MAX;

            for (int i = 0; i < inputMappings.size(); i++) {
                swapPath.clear();   // for 1 SP
                allSPFSwapPaths.clear();    // for all SPs
                // find min cost among all shortest paths for a particular input mapping for the current instruction
                unsigned int costCurrentMapping = findSwapsFor1Instruction(currentInstruction,
                                                             quArchitecture.getCouplingMap()); // acc. to current mapping before perm
                mappingWiseShortestPaths.push_back(allSPFSwapPaths);    // all SPFs for current instruction & current mapping

                if (costCurrentMapping < minCostInstruction) {
                    minCostInstruction = costCurrentMapping;
                    minSwapInstruction = swaps;
                    minHadamardInstruction = hadamards;
                    Util::println("<<<< instruction minimum!");
                } else Util::println("");
                perInstructionMappingCounter++; // needed in getCurrentMapping()
            }

            if (minCostInstruction <= minCostLayer){
                minCostLayer = minCostInstruction;
                minSwapLayer = minSwapInstruction;
                minHadamardLayer = minHadamardInstruction;
                minMappingWiseShortestPathsLayer = mappingWiseShortestPaths;
                minMappingWiseShortestPathCostsLayer = mappingWiseShortestPathCosts;  // rhs populated in caterhadamard method
                minId = id;
                Util::setVerbose();
                Util::println("<<<< layer minimum!");
                Util::resetVerbose();
            }

            Util::setVerbose();
            if (Util::verbose){
                cout << "Instr Id: " << id << " Min cost: " + to_string(minCostInstruction) + ", Min swaps: " + to_string(minSwapInstruction) +
                                              ", Min hadamards: " + to_string(minHadamardInstruction) << endl;
            }
            Util::resetVerbose();

        }

        for (QuGate *inst: nonUnaryInstructions) { // todo make it efficient by using hashmap
            if (inst->getGateId() == minId) {
                currentInstruction = inst;
                break;
            }
        }

        if (previousInstruction != nullptr && (previousInstruction->isDitto(currentInstruction))) {
            instructionWiseMappings.push_back(instructionWiseMappings[instructionWiseMappings.size()-1]);
            updateMappingIdsForDitto();
            Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
            selectedNonUnaryInstructionIds.push_back(minId);
            QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
            layerManager->removeInstruction(minId);
            programCounter++;
            continue;
        }


        Util::setVerbose();
        if (Util::verbose) {
            cout << "Selected Instr Id: " << minId
                 << " Min cost: " + to_string(minCostLayer) + ", Min swaps: " + to_string(minSwapLayer) +
                    ", Min hadamards: " + to_string(minHadamardLayer) << endl;
        }
        Util::resetVerbose();
        selectedNonUnaryInstructionIds.push_back(minId);

        mappingWiseShortestPaths = minMappingWiseShortestPathsLayer;
        mappingWiseShortestPathCosts = minMappingWiseShortestPathCostsLayer;
        QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
        layerManager->removeInstruction(minId);

        totalCost += minCostLayer;
        totalSwaps += minSwapLayer;
        totalHadamards += minHadamardLayer;  // todo totalHadamards value not correct..

        prepareMappingsForNextInstruction(inputMappings, mappingWiseShortestPaths, minCostLayer, quArchitecture);

        Util::setVerbose();
        if (Util::verbose)
            cout << "Current Instruction: " << *currentInstruction << endl;
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        Util::resetVerbose();
        previousInstruction = currentInstruction;
        programCounter++;
    }

    delete allSPF;
    delete allPairShortestPathFinder;
    QuCircuitLayerManager::deleteInstance();

    Util::setVerbose();
    Util::println(circuit.getFileName() + ": " + to_string(totalCost));
    Util::resetVerbose();

    return totalCost;
}


int QuSmartSwapper::findTotalSwapsDefault(QuArchitecture& quArchitecture) {
    unsigned int totalCost = 0;
    swaps = 0;
    totalSwaps = 0;
    totalHadamards = 0;

    allSPF = new AllShortestPathsFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());
    allPairShortestPathFinder = new AllPairShortestPathFinder(quArchitecture.getCouplingMap(), quArchitecture.getN());

    removeUnaryInstructions();

    QuGate* previousInstruction = nullptr;
    for(QuGate* currentInstruction: nonUnaryInstructions){
        Util::println("INSTRUCTION ANALYSIS START: " + to_string(programCounter));
        this->currentInstruction = currentInstruction;
        if(currentInstructionSameAsPrevious(previousInstruction, currentInstruction))
            continue;

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();

        perInstructionMappingCounter = 0;   // needed in getCurrentMapping()
        mappingWiseShortestPathCosts.clear();
        mappingWiseShortestPathCosts.resize(inputMappings.size());
        mappingWiseShortestPaths.clear();

        unsigned int minCost = INT32_MAX;
        unsigned int minSwap = INT32_MAX;
        unsigned int minHadamard = INT32_MAX;

        for(int i=0; i<inputMappings.size(); i++){
            swapPath.clear();   // for 1 SP
            allSPFSwapPaths.clear();    // for all SPs
            // find min cost among all shortest paths for a particular input mapping for the current instruction
            unsigned int costCurrentMapping = findSwapsFor1Instruction(currentInstruction, quArchitecture.getCouplingMap());
            mappingWiseShortestPaths.push_back(allSPFSwapPaths);    // all SPFs for current instruction & current mapping

            if (costCurrentMapping < minCost) {
                minCost = costCurrentMapping;
                minSwap = swaps;
                minHadamard = hadamards;
                Util::println("<<<< minimum!");
            }
            else Util::println("");
            perInstructionMappingCounter++; // needed in getCurrentMapping()
        }

        totalCost += minCost;
        totalSwaps += minSwap;
        totalHadamards += minHadamard;  // todo totalHadamards value not correct..

        prepareMappingsForNextInstruction(inputMappings, mappingWiseShortestPaths, minCost, quArchitecture);

        doExtraHadamardFiltering(currentInstruction, quArchitecture);

        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
        previousInstruction = currentInstruction;
    }
    delete allSPF;
    delete allPairShortestPathFinder;

    return totalCost;
}

int QuSmartSwapper::findTotalSwaps(QuArchitecture& quArchitecture) {
    unsigned int totalCost = 0;
    if (DAG_SCHEME){
        totalCost = findTotalSwapsDAG(quArchitecture);
    }
    else {
        totalCost = findTotalSwapsDefault(quArchitecture);
    }
    return totalCost;
}

// find all mappings for current instruction
vector<QuMapping> QuSmartSwapper::findAllMappingsFromPermutations(QuMapping& inputMapping, vector<int> swapSequence, QuArchitecture& quArchitecture) {
    vector<QuMapping> mappings;
    unsigned int totalMoves = swapSequence.size() - 2;
    vector<int> quBitIndexes = currentInstruction->getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];
    int srcMoves = 0;
    int destMoves = 0;
    if(totalMoves > 0){
        for (unsigned int i = 0; i <= totalMoves; i++) {
            QuMapping mapping = inputMapping;
            vector<Swap> swapGates;
            vector<int> srcSeq, destSeq, tempSeq;
            srcMoves = totalMoves - i;
            destMoves = i;
            srcSeq.push_back(mapping.getPhysicalBit(src));
            for (int j = 0; j < srcMoves; j++) {
                int val = swapSequence.at(j + 1);
                srcSeq.push_back(val);
            }
            destSeq.push_back(mapping.getPhysicalBit(dest));
            for (int j = 0; j < destMoves; j++) {
                destSeq.push_back(swapSequence[totalMoves - j]);
            }
            // add swap instructions to mapping
            vector<Swap> swapGatesTemp;
            if (srcSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(srcSeq);
                swapGates.insert(swapGates.begin(), swapGatesTemp.begin(), swapGatesTemp.end());
            }
            if (destSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(destSeq);
                swapGates.insert(swapGates.end(), swapGatesTemp.begin(), swapGatesTemp.end());
            }
            if(swapGates.size()>0) {
                mapping.setSwapInstructions(swapGates);
                mappings.push_back(mapping);
            }
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

vector<QuGate*> QuSmartSwapper::getKRestrictInstructions(int k){
    vector<QuGate*> instructions;
    if (k > nonUnaryInstructions.size())
        k = nonUnaryInstructions.size();
    if (k > 0) {
        if (DAG_SCHEME) {
            vector<QuGate *> temp;
            QuCircuitLayerManager *layerManager = QuCircuitLayerManager::getInstance(temp, 0);
            vector<int> instructionIds = layerManager->getFirstKInstructionIds(k);
            for (auto id: instructionIds) {
                for (QuGate *inst: nonUnaryInstructions) {
                    if (inst->getGateId() == id) {
                        instructions.push_back(inst);
                    }
                }
            }
        } else {
            int i = 1;
            instructions.push_back(nonUnaryInstructions[0]);
            while (!instructions.empty() && i<nonUnaryInstructions.size() && instructions.size() < k) {
                if (isNewInsturction(nonUnaryInstructions[i], instructions))
                    instructions.push_back(nonUnaryInstructions[i]);
                i++;
            }
        }
    }
    return instructions;
}
//
//bool QuSmartSwapper::pred(const QuGate* a, const QuGate* b){
//    return ((a->getArgAtIndex(0) == b->getArgAtIndex(0)) && (a->getArgAtIndex(1) == b->getArgAtIndex(1))
//         || (a->getArgAtIndex(0) == b->getArgAtIndex(1)) && (a->getArgAtIndex(1) == b->getArgAtIndex(0)));
//}

bool QuSmartSwapper::isNewInsturction(QuGate* currentInstruction, vector<QuGate*>& instructions) {
    for (QuGate* oldInstruction: instructions) {
        if ((*oldInstruction) == (*currentInstruction))
            return false;
    }
    return true;
}


pair<vector<pair<int, int>>, vector<pair<int, int>>> QuSmartSwapper::makeRestrictionPairList(int k){ // top k unique instructions
    vector<pair<int, int>> restrictedPairs;
    vector<pair<int, int>> restrictedPairSources;
    int j = 0;
    bool dup = false;

    vector<QuGate*> instructions = getKRestrictInstructions(k);

    int x = k; // k is max restrictions
//    for (int i = 0; i < k && j<nonUnaryInstructions.size(); ++i) {
//    for (int i = 0; i < k; ++i) {
    for (int i = 0; i < k; ++i) {
        dup = false;
        QuGate* gate = instructions[i];
//        cout << i << " " << j << " " << k << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
//        cout << *gate << endl;
        pair<int, int> newPair = make_pair(gate->getArgAtIndex(0), gate->getArgAtIndex(1));
        restrictedPairSources.push_back(newPair);
        pair<int, int> newPairInverted = make_pair(gate->getArgAtIndex(1), gate->getArgAtIndex(0));
        for (int l = 0; l < restrictedPairs.size(); ++l) {
            if (newPair == restrictedPairs[l] || newPairInverted == restrictedPairs[l]) {
                dup = true;
                break;
            }
            else
                j++;
        }
        if (!dup)
            restrictedPairs.push_back(newPair);
//        cout << "k = " << k;
//        cout << ", j = " << j << endl;
//        cout << ", restrictedPairs.size() = " << restrictedPairs.size() << endl;
        if (j>=k) break;

    }
    // restrictedPairs: unique pairs; restrictedPairSources: all pairs => pairs = binary cnot gate args (src , target)
    return make_pair(restrictedPairs, restrictedPairSources);
}

vector<QuMapping> QuSmartSwapper::generateInitialMappings() {
//    mappingInitializer.initGenerator(architecture.getN());
    vector<pair<int, int>> restrictionList;
    vector<pair<int, int>> restrictionListSources;

      // set k unique instructions to restrict // todo SHA: no restrictions
      if (INIT_MAPPING_RESTRICT_MODE) {
          pair<vector<pair<int, int>>, vector<pair<int, int>>> restrictionData = makeRestrictionPairList(K);
          restrictionList = restrictionData.first;
          restrictionListSources = restrictionData.second;
      }
          //    vector<pair<int, int>> restrictionList = makeRestrictionPairList(architecture.getN()/2);
      initialMappings = mappingInitializer.generateSmartMappings(restrictionListSources, restrictionList,
                                                                     architecture);
    return initialMappings;
}

vector<QuMapping> QuSmartSwapper::getAllMappingsForCurrentInstruction() {
    vector<QuMapping> mappings;
    string pid = "*";
    if (!programCounter) {  // 1st instruction
        mappings = generateInitialMappings();
    } else {
        mappings = instructionWiseMappings[programCounter-1];
    }
    int i = 0;
    for (auto& mapping: mappings) {
        if(programCounter>0){
            mapping.setParentMappingId(mapping.getMappingId());
        }
        else {
            mapping.setParentMappingId(pid);
        }
        mapping.setMappingId(to_string(programCounter) + "." + to_string(i));
        i++;
    }

    if (RANDOM_SAMPLING_MAPPINGS_PRUNING) {
        // todo divide threshold among mappings
        unsigned long threshold = MAPPING_THRESHOLD - 1;
        if (mappings.size() <= threshold)
            threshold = mappings.size();
        unordered_set<int> sampleIndexes = Util::getNRandomIndexes(threshold, mappings.size());
        vector<QuMapping> sampledMappings(threshold);
        int i = 0;
        for (auto index: sampleIndexes) {
            sampledMappings[i] = mappings[index];
            i++;
        }
        mappings = sampledMappings;
    }
    else {
        if(mappings.size() > MAPPING_THRESHOLD) {
            mappings.erase(mappings.begin() + MAPPING_THRESHOLD, mappings.end());
        }
    }
    return mappings;
}

QuMapping QuSmartSwapper::getCurrentMapping() {
    QuMapping currentMapping;
    if(!programCounter)
        currentMapping = initialMappings[perInstructionMappingCounter];
    else
        currentMapping = instructionWiseMappings[programCounter-1][perInstructionMappingCounter];
    return currentMapping;
}

// returns the minimum total cost of all shortest paths before permutations
int QuSmartSwapper::findSwapsFor1Instruction(QuGate *currentInstruction, int **couplingMap) {
    vector<int> quBitIndexes = currentInstruction -> getArgIndex(); // logical qubit index values
    swaps = 0;
    vector<int> shortestPath;
//    int edgeDistance = 0;

    QuMapping mapping = getCurrentMapping();
    cout << "mappping: " << mapping.toString() << endl;

    int src = mapping.getPhysicalBit(quBitIndexes[0]);
    int dest = mapping.getPhysicalBit(quBitIndexes[1]);

    // returns the shortest path and the cost as a pair
    pair<vector<int>, int> result = allPairShortestPathFinder->getSPSequence(src, dest);
    shortestPath = result.first;
//    edgeDistance = result.second; // # of swaps + 1 OR # of hops OR edge distance
    swaps = shortestPath.size() - 2;
    if (swaps > 0) // find all shortest paths
        allSPFSwapPaths = allSPF->findSingleSourceAllShortestPaths(src, dest, swaps); // todo find shortest w/o swaps arg
    else
        allSPFSwapPaths.push_back(shortestPath);
    return findShortestPathsMinimumCost();
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
        : QuSwapStrategy(circuit, architecture), initialMapping(circuit.getRows()), perInstructionMappingCounter(0), allSPF(
        nullptr), allPairShortestPathFinder(nullptr), mappingInitializer(architecture.getN(), circuit.getN()), hadamards(0), swaps(0), totalSwaps(0), totalHadamards(0),
        currentInstruction(nullptr) {}

//void QuSmartSwapper::setInitialMapping(){
//    initialMapping = mappingInitializer.getNextMapping();
//}

unsigned int QuSmartSwapper::constraintNotSatisfied(int src, int dest, int **couplingMap) {
//    Util::println("src: " + to_string(src));
//    Util::println("dest: " + to_string(dest));
//    Util::print("couplingMap[src][dest]: ");
//    cout << couplingMap[src][dest] << endl;
    if(couplingMap[src][dest] == -1)
        return 1;
    return 0;
}

void QuSmartSwapper::generateOptimalInstructions(QuArchitecture& quArchitecture) {
    int nonUnarySize = instructionWiseMappings.size() - 1;
    string parentMappingId;
    vector<QuGate *> finalProgram;
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

//    cout << "instructionWiseMappings.size() :" << instructionWiseMappings.size() << endl;
//    cout << "nonUnaryInstructions.size() :" << nonUnaryInstructions.size() << endl;

//    for(QuMapping m: mappings){
//        cout << "m.getSwapInstructions().size(): " << m.getSwapInstructions().size() << endl;
//        for(QuGate* g: m.getSwapInstructions()){
//            cout << (*g) << endl;
//        }
//    }

    if (!nonUnaryInstructions.empty()){  // todo SHA: use DAG sequence
        QuMapping theMapping = instructionWiseMappings[nonUnarySize][0]; // get any (1st) last mapping to start backtracking
        vector<QuMapping> selectedMappings;
        Util::println("A: theMapping.getMappingId(): " + theMapping.getMappingId());
        Util::println("A: theMapping.getParentMappingId(): " + theMapping.getParentMappingId());

        int i;
        for (i = nonUnarySize - 1; i >= 0; i--) {
            selectedMappings.insert(selectedMappings.begin(), theMapping);
            parentMappingId = theMapping.getParentMappingId();
            Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
            Util::println("parentProgramCounter: " + to_string(parentProgramCounter));
            Util::println("parentMappingCounter: " + to_string(parentMappingCounter));
            Util::println("instructionWiseMappings[parentProgramCounter].size(): " +
                          to_string(instructionWiseMappings[parentProgramCounter].size()));
    //        vector<QuGate*> swapInstructions = theMapping.getSwapInstructions();
    //        for(int i=0; i<swapInstructions.size(); i++) {
    //            cout << *swapInstructions[i] << endl;
    //        }
    //        theMapping.print();
    //        cout << *nonUnaryInstructions[i+1] << endl;
    //        cout << endl;
            theMapping = instructionWiseMappings[parentProgramCounter][parentMappingCounter];
            Util::println("B: theMapping.getMappingId(): " + theMapping.getMappingId());
            Util::println("B: theMapping.getParentMappingId(): " + theMapping.getParentMappingId());
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
        swaps = 0;
        for (i = 0; i < selectedMappings.size(); i++) {
            selectedMappings[i].print();
            Util::println("selectedMappings[i].getMappingId(): " + selectedMappings[i].getMappingId());
            Util::println("selectedMappings[i].getParentMappingId(): " + selectedMappings[i].getParentMappingId());

            x = insertRemovedUnaryInstructions(finalProgram, x);

            vector<QuGate *> swapInstructions;// = selectedMappings[i].getSwapInstructions();
            for (int j = 0; j < selectedMappings[i].getSwapInstructions().size(); j++) {
                QuGate *swapGate = QuGateFactory::getQuGate("SWAP");
                swapGate->setArgAtIndex(0, selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0]);
                swapGate->setArgAtIndex(1, selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1]);
                swapInstructions.push_back(swapGate);
                swaps++;
            }
            for (int i = 0; i < swapInstructions.size(); i++) {
    //            cout << *swapInstructions[i] << endl;
                finalProgram.push_back(swapInstructions[i]);
            }
    //        cout << *nonUnaryInstructions[i] << endl;
    //        finalProgram.push_back(nonUnaryInstructions[i]);

            hadamardCheck(finalProgram, quArchitecture, selectedMappings[i], i);
//            circuit.setHadamards(hadamards);
    //        cout << endl;
        }

        Util::println("selectedMappings.size(): " + to_string(selectedMappings.size()));
        Util::println("nonUnaryInstructions.size(): " + to_string(nonUnaryInstructions.size()));

    }

    insertEndingUnaryInstructions(finalProgram);

//    optimize(finalProgram);

    circuit.setInstructionsV1(finalProgram);
    circuit.setSwaps(totalSwaps);  // todo  should it be swaps??
    circuit.setHadamards(totalHadamards);

//    cout << circuit.getFileName() << " - original circuit size: " << circuit.getInstructions().size() << endl;
//    cout << circuit.getFileName() << " - swaps: " << totalSwaps << endl;
//    cout << circuit.getFileName() << " - hadamards: " << hadamards << endl;  // todo value not correct
//    cout<< circuit.getFileName() << " - final circuit size (orig + hadamard + swap gates (non-elementary)): " << circuit.getInstructionsV1().size() << endl;
//    cout << *nonUnaryInstructions[i] << endl;

//    circuit.getInstructionsV1().push_back(currentInstruction);

}

void QuSmartSwapper::optimize(vector<QuGate*>& finalProgram){
    int cancellations = performCNOTCancellations(finalProgram);
    cout << "CNOT cancellations: " << cancellations << endl;
    cancellations = performUnaryCancellations(finalProgram);
    cout << "Unary cancellations: " << cancellations << endl;
}

void QuSmartSwapper::insertEndingUnaryInstructions(vector<QuGate*>& finalProgram) {
    vector<QuGate*> originalProgram = circuit.getInstructions();
    int i=originalProgram.size()-1;
    while(i>=0 && originalProgram[i]->isUnary()){
        i--;
//        cout << "unary" << endl;
    }
    if (i!=originalProgram.size()-1)
        finalProgram.insert(finalProgram.end(), originalProgram.begin()+i+1, originalProgram.end());
}

int QuSmartSwapper::insertRemovedUnaryInstructions(vector<QuGate*>& finalProgram, int nextNonUnaryIndex) {
    vector<QuGate*> originalProgram = circuit.getInstructions();
    int i = 0;
    int nonUnaryCounter = 0;
    originalProgram.erase(originalProgram.begin(), originalProgram.begin() + nextNonUnaryIndex);
    while(originalProgram[i]->isUnary()){
        finalProgram.push_back(originalProgram[i]);
        i++;
//        cout << "originalProgram.size(): " << originalProgram.size() << ", i: " << i << endl;
    }
    Util::println("nextNonUnaryIndex + i + 1: " + to_string(nextNonUnaryIndex + i + 1));
    return nextNonUnaryIndex + i + 1;

}

void QuSmartSwapper::hadamardCheck(vector<QuGate *> &finalProgram, QuArchitecture &quArchitecture, QuMapping &currentMapping,
                              int index) {
    vector<QuGate*> instructions;
    QuGate* currentInstruction = nullptr;
    if (DAG_SCHEME) {
        for (auto id: selectedNonUnaryInstructionIds) {
            for (auto instr: nonUnaryInstructions) {
                if (id == (*instr).getGateId()) {
                    instructions.push_back(instr);
                    break;
                }
            }
        }
        currentInstruction = instructions[index];
    }
    else {
        currentInstruction = nonUnaryInstructions[index];
    }
//    QuGate* currentInstruction = nonUnaryInstructions[index];

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
    if (DAG_SCHEME) {
        finalProgram.push_back(instructions[index]); // todo swap src and target?
    }
    else {
        finalProgram.push_back(nonUnaryInstructions[index]);
    }
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

void QuSmartSwapper::mappingSanityCheck() {
    int nonUnarySize = instructionWiseMappings.size()-1;
    string parentMappingId;
    int parentProgramCounter, parentMappingCounter;


    vector<QuMapping> selectedMappings;

    Util::println("C: theMapping.getMappingId(): " + instructionWiseMappings[0][0].getMappingId());
    Util::println("C: theMapping.getParentMappingId(): " + instructionWiseMappings[0][0].getParentMappingId());

    QuMapping theMapping;

    for (int j = 0; j < instructionWiseMappings[nonUnarySize-1].size(); ++j) {  // last layer
        int i = nonUnarySize-1;
        int s = 0;
        parentProgramCounter = i;
        parentMappingCounter = j;
        while (i>=0){  // go up the layers
            theMapping = instructionWiseMappings[parentProgramCounter][parentMappingCounter];
            parentMappingId = theMapping.getParentMappingId();
            Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
            s += theMapping.getSwapInstructions().size();
            i--;
        }
        Util::println("swaps: " + to_string(s));
    }
}

int QuSmartSwapper::calculateHadamardCost(vector<int> shortestPath, int **couplingMap) {
    int hadamards = 0;
    for (int i = 0; i < shortestPath.size() - 1; ++i) {
        if (couplingMap[shortestPath[i]][shortestPath[i+1]] == -1)
            hadamards += 4;
    }
    return hadamards;
}

int QuSmartSwapper::findShortestPathsMinimumCost() {
    int minCost = INT_MAX;
    for (auto& path: allSPFSwapPaths) {
        int hadamardCost = calculateHadamardCost(path, architecture.getCouplingMap());
        int swapCost = (path.size() - 2) * 7;
        int totalCost = hadamardCost + swapCost;

        mappingWiseShortestPathCosts[perInstructionMappingCounter].push_back(totalCost);
        if (totalCost < minCost) {
            minCost = totalCost;
            hadamards = hadamardCost;
        }
    }
    return minCost;
}

void QuSmartSwapper::prepareMappingsForNextInstruction(vector<QuMapping> &inputMappings, vector<vector<vector<int>>>& mappingWiseShortestPaths, unsigned int min, QuArchitecture& quArchitecture) {
    vector<QuMapping> nextInstructionMappings;
    vector<vector<vector<int>>> filteredMappingWiseShortestPaths;
    vector<QuMapping> filteredInputMappings;

    for(int i=0; i<inputMappings.size(); i++){
        vector<int> temp = mappingWiseShortestPathCosts[i];
//        sort(temp.begin(), temp.end());
        int currentCost = temp[i];
        if (!mappingWiseShortestPaths[i].empty() && (currentCost == min)) {
            filteredInputMappings.push_back(inputMappings[i]);
            filteredMappingWiseShortestPaths.push_back(mappingWiseShortestPaths[i]);
        }
    }

    vector<QuMapping> temp;
    for (unsigned int j = 0; j < filteredInputMappings.size(); j++) {  // todo check why default mapping not filtered
        vector<QuMapping> allPermutationMappings;
        filteredInputMappings[j].clearSwapInstructions();

        for (unsigned int k = 0; k < filteredMappingWiseShortestPaths[j].size(); k++) {
            //todo legit mystery?!
            string pathString = Util::pathToString(filteredMappingWiseShortestPaths[j][k]);
            filteredInputMappings[j].print();
            if (filteredMappingWiseShortestPaths[j][k].size() == 2) {
                temp.insert(temp.end(), filteredInputMappings[j]);
            }
            else {
                allPermutationMappings = findAllMappingsFromPermutations(filteredInputMappings[j],
                                                                         filteredMappingWiseShortestPaths[j][k],
                                                                         quArchitecture);
                temp.insert(temp.end(), allPermutationMappings.begin(), allPermutationMappings.end());
            }
        }
    }

    Util::println("# of mappings from permutations: (temp.size) "  + to_string(temp.size()));
    for(int i=0; i<temp.size(); i++){
        if(programCounter > 0) {
            temp[i].setParentMappingId(temp[i].getMappingId());
            temp[i].setMappingId(to_string(programCounter) + "." + to_string(i));
        }
        cout << "PPPPPPPPPPPPPPPPPPPPPPPP" << "temp[i].getMappingId(): " << temp[i].getMappingId();
        cout << " || temp[i].getParentMappingId(): " << temp[i].getParentMappingId() << " ";
        cout << temp[i].toString() << endl;
        cout << " || programCounter: " << programCounter;
        cout << " || mapping counter: " << i << endl;

    }

    if(!temp.empty()){
        Util::println("mappings generated for next instruction: size = " + to_string(temp.size()));
        nextInstructionMappings.insert(nextInstructionMappings.end(), temp.begin(), temp.end());
    }

    if(!nextInstructionMappings.empty()) {
        instructionWiseMappings.push_back(nextInstructionMappings);
    }
}

int QuSmartSwapper::performCNOTCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
    int src1 = -1, target1 = -1, src2 = -1, target2 = -1, src3 = -1, target3 = -1;
    bool cancelIt = true;
    int i = 0, j = 0, k = 0;
    vector<int> cancelIndexes;
    auto it1 = finalProgram.begin();
//        if(it->first == physicalQuBit || it->second == physicalQuBit)
//            it = couples.erase(it);
//        else
//            ++it;
//    }
    while(it1!=finalProgram.end()){
//    for (i = 0; i < finalProgram.size(); ++i) {
        if ((*it1)->getMnemonic() == "cx"){
            src1 = (*it1)->getArgAtIndex(0);
            target1 = (*it1)->getArgAtIndex(1);
            auto it2 = it1 + 1;
            while(it2!=finalProgram.end()){
//            for (j = i + 1; j < finalProgram.size(); ++j) {
                if ((*it2)->getMnemonic() == "cx") {
                    src2 = (*it2)->getArgAtIndex(0);
                    target2 = (*it2)->getArgAtIndex(1);
                    if (src1 == src2 && target1 == target2){
                        auto it3 = it1 + 1;
                        int temp1 = 0;
                        while(it3 != it2){
//                        for (k = i+1; k < j; ++k) {
                            if ((*it3)->getMnemonic() == "cx") {
                                src3 = (*it3)->getArgAtIndex(0);
                                target3 = (*it3)->getArgAtIndex(1);
                                if(src1 == target3 || target1 == src3){
                                    cancelIt = false;
                                }
                            }
                            it3++;
                        }
                    }
                }
                it2++;
                j++;
            }
            if(cancelIt) {
                cancelIndexes.push_back(i);
                cancelIndexes.push_back(j);
                finalProgram.erase(finalProgram.begin() + i);
                finalProgram.erase(finalProgram.begin() + j - 1);

//                i = j + 1;
            }
        }
        i++;
        it1++;
    }
    // todo remove cancelled CNOTS from finalProgram
    return cancelIndexes.size();
}
//int QuSmartSwapper::performCNOTCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
//    int src1, target1, src2, target2, src3, target3;
//    bool cancelIt = true;
//    int i = 0, j = 0, k = 0;
//    vector<int> cancelIndexes;
//    auto it1 = finalProgram.begin();
////        if(it->first == physicalQuBit || it->second == physicalQuBit)
////            it = couples.erase(it);
////        else
////            ++it;
////    }
//    while(it1!=finalProgram.end()){
////    for (i = 0; i < finalProgram.size(); ++i) {
//        if (finalProgram[i]->getMnemonic() == "cx"){
//            src1 = finalProgram[i]->getArgAtIndex(0);
//            target1 = finalProgram[i]->getArgAtIndex(1);
//            auto it2 = it1 + 1;
//            while(it2!=finalProgram.end()){
////            for (j = i + 1; j < finalProgram.size(); ++j) {
//                if (finalProgram[j]->getMnemonic() == "cx") {
//                    src2 = finalProgram[j]->getArgAtIndex(0);
//                    target2 = finalProgram[j]->getArgAtIndex(1);
//                    if (src1 == src2 && target1 == target2){
//                        for (k = i+1; k < j; ++k) {
//                            if (finalProgram[k]->getMnemonic() == "cx") {
//                                src3 = finalProgram[j]->getArgAtIndex(0);
//                                target3 = finalProgram[j]->getArgAtIndex(1);
//                            }
//                            if(src1 == target3 || target1 == src3){
//                                cancelIt = false;
//                            }
//                        }
//                    }
//                }
//                j++;
//            }
//            if(cancelIt) {
//                cancelIndexes.push_back(i);
//                cancelIndexes.push_back(j);
//                finalProgram.erase(finalProgram.begin() + i);
//                finalProgram.erase(finalProgram.begin() + j - 1);
//
////                i = j + 1;
//            }
//        }
//        i++;
//    }
//    // todo remove cancelled CNOTS from finalProgram
//    return cancelIndexes.size();
//}

int QuSmartSwapper::performUnaryCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent single qubits and check in-between
    int cancellations = 0;
    auto it1 = finalProgram.begin();
    while (it1 != finalProgram.end()){
        bool isUnary1 = (*it1)->getCardinality() == 1;
        if (isUnary1) {
            auto it2 = findMergingPartner(it1, finalProgram.end());
            if (it2 != finalProgram.end() && it2 != it1) {
                finalProgram.erase(it1);
                cancellations++;
//                it1 = it2;
            }
            else{
                it1++;
            }
        }
        else{
            it1++;
        }
    }
    return cancellations;
//        if (finalProgram[i]->getMnemonic() == "rz" && finalProgram[i + 1]->getMnemonic() == "rz" &&
//            finalProgram[i]->getArgAtIndex(0) == finalProgram[i + 1]->getArgAtIndex(0)) {
}

vector<QuGate *>::iterator QuSmartSwapper::findMergingPartner(vector<QuGate *>::iterator it1, vector<QuGate *>::iterator end) {
    vector<QuGate *>::iterator it2 = it1++;
    int quBit1 = (*it1)->getArgAtIndex(0);
    bool isUnary2;
    int quBit2 = -1;
    do{
        isUnary2 = (*it2)->getCardinality() == 1;
        quBit2 = (*it2)->getArgAtIndex(0);
        if (quBit1 == quBit2){
            if (isUnary2) {
                break;
            }
            else {
                int quBit2Target = (*it2)->getArgAtIndex(1);
                it2 = it1;
                break;
            }
        }
        it2++;
    } while (quBit1 != quBit2 && it2 != end);
    return it2;
}

vector<int> QuSmartSwapper::getCurrentInstructionIds() {
    QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
    vector<int> instructionIds = layerManager->getNextSourceInstructionIds();

    return instructionIds;
}

void QuSmartSwapper::updateMappingIdsForDitto() {
    auto& childMappings = instructionWiseMappings[instructionWiseMappings.size()-1];
    auto& parentMappings = instructionWiseMappings[instructionWiseMappings.size()-2];
    int i = 0;
    for(auto& mapping: childMappings){
        mapping.clearSwapInstructions();
        mapping.setParentMappingId(parentMappings[i].getMappingId());
        mapping.setMappingId(to_string(programCounter) + "." + to_string(i));
        i++;
    }
    cout << "temp" << endl;
}

bool QuSmartSwapper::currentInstructionSameAsPrevious(QuGate *previous, QuGate *current) {
    if (previous != nullptr && (previous->isDitto(currentInstruction))) {
        instructionWiseMappings.push_back(instructionWiseMappings[instructionWiseMappings.size()-1]);
        updateMappingIdsForDitto();
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
        return true;
    }
    return false;
}

void QuSmartSwapper::doExtraHadamardFiltering(QuGate *currentInstruction, QuArchitecture &quArchitecture) {
    auto srcLogical = currentInstruction->getArgAtIndex(0);
    auto targetLogical = currentInstruction->getArgAtIndex(1);

    auto mappings = instructionWiseMappings[instructionWiseMappings.size()-1];
    cout << "mappings size old: " << mappings.size() << endl;
    auto it = mappings.begin();
    while(it != mappings.end()){
        cout << it->toString() << endl;
        if(!quArchitecture.isCompatable(it->getPhysicalBit(srcLogical), it->getPhysicalBit(targetLogical))){
            cout << "YAYAYAYYYAYAYAYAYYA : "  << srcLogical << " => " << targetLogical << " not compatable!!" << endl;
            it = mappings.erase(it);
        }
        else
            ++it;
    }
    if (!mappings.empty()) {
        cout << "empty" << endl;
        instructionWiseMappings[instructionWiseMappings.size()-1] = mappings;
    }

    cout << "mappings size new: " << mappings.size();
    cout << endl;
}
