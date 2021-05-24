//
// Created by SHA on 11/19/19.
//

#include "QuSmartSwapper.h"
#include "QuCircuitOptimizer.h"
#include "QuGateFactory.h"
#include "QuMappingInitializer.h"
#include "QuCircuitLayerManager.h"
#include "algorithm/AllShortestPathsFinder.h"
#include "util/Util.h"
#include "util/Constants.h"
#include "algorithm/AllPairShortestPathFinder.h"
#include <algorithm>
#include <climits>
#include <iostream>

vector<shared_ptr<QuGate>> QuSmartSwapper::removeUnaryInstructions(){
    vector<shared_ptr<QuGate>> instructions = circuit.getInstructions0();
    for(shared_ptr<QuGate> currentInstruction: instructions){
        if(currentInstruction->getCardinality()> 1) {
            nonUnaryInstructions.push_back(currentInstruction);
        }
    }
    return nonUnaryInstructions;
}

int QuSmartSwapper::findTotalCostDAG() {
    unsigned int totalCost = 0;
    init();
    removeUnaryInstructions();
//    shared_ptr<QuGate> previousInstruction = nullptr;

    int totalInstructions = nonUnaryInstructions.size();
    QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
    nonUnaryInstructionsMap = layerManager->getInstructionMap();

    for(int i=0; i<totalInstructions; i++){
//        Util::setVerbose();
        Util::println("INSTRUCTION ANALYSIS START: programCounter: " + to_string(programCounter));
//        Util::resetVerbose();

         this->currentInstruction = nullptr;

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();  // todo get mappings only once esp initial not re-generate
        currentInstructionIds = getCurrentInstructionIds(); // source instructions (independent)
        unsigned int minId = INT32_MAX;
        unsigned int minCostLayer = INT32_MAX;
        vector<vector<vector<int>>> minMappingWiseShortestPathsLayer;
        vector<vector<int>> minMappingWiseShortestPathCostsLayer;
        vector<QuMapping> nextInstructionMappings;
        vector<QuMapping> minFilteredSPMappingsForAllMappings;

        for (auto id: currentInstructionIds) {
            currentInstruction = nonUnaryInstructionsMap[id];
            findShortestPathsForAllInputMappings(inputMappings);
//            cout << *currentInstruction << endl;
            auto minData = findMinCostMappingsForNextInstruction(inputMappings);
            auto minCostInstruction = minData.first;
            auto nextInstructionMappings = minData.second;

            if (minCostInstruction < minCostLayer){
                minCostLayer = minCostInstruction;
                minId = id;
                minFilteredSPMappingsForAllMappings = nextInstructionMappings;
            }
        }

        // DAG
//        if (previousInstruction != nullptr && (previousInstruction->isDitto(currentInstruction))) {
//            instructionWiseMappings.push_back(instructionWiseMappings[instructionWiseMappings.size()-1]);
//            updateMappingIdsForDitto();
//            Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
//            selectedNonUnaryInstructionIds.push_back(minId);
//            QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
//            layerManager->removeInstruction(minId);
//            programCounter++;
//            continue;
//        }

        if(!minFilteredSPMappingsForAllMappings.empty()){
            auto totalMappingsCounter = 0;
            for (auto& mapping: minFilteredSPMappingsForAllMappings) {
                mapping.setMappingId(to_string(programCounter+1) + "." + to_string(totalMappingsCounter));
                totalMappingsCounter++;
            }
            instructionWiseMappings.push_back(minFilteredSPMappingsForAllMappings);
        }
        else {
            cout << "WWWWWWWWWWWWHHHHHHHHHHHHHHHYYYYYYYYYYYYYYYYYY!" << endl;
        }

        selectedNonUnaryInstructionIds.push_back(minId);
        layerManager->removeInstruction(minId);

        totalCost += minCostLayer;

//        Util::setVerbose();
        Util::println("minCostLayer: " + to_string(minCostLayer) + " totalCost: " + to_string(totalCost));
        Util::println("INSTRUCTION ANALYSIS END: programCounter: " + to_string(programCounter) + " gateId: " + to_string(minId));
//        Util::resetVerbose();
//        previousInstruction = currentInstruction;
        programCounter++;
    }

    delete allSPF;
    delete allPairShortestPathFinder;
    QuCircuitLayerManager::deleteInstance();

    return totalCost;
}

void QuSmartSwapper::init(){
    swaps = 0;
//    totalSwaps = 0;
//    totalHadamards = 0;

    allSPF = new AllShortestPathsFinder(architecture.getCouplingMap(), architecture.getN());
    allPairShortestPathFinder = new AllPairShortestPathFinder(architecture.getCouplingMap(), architecture.getN());
}

void QuSmartSwapper::findShortestPathsForAllInputMappings(vector<QuMapping> inputMappings) {
    mappingWiseShortestPathCosts.clear();
    mappingWiseShortestPathCosts.resize(inputMappings.size());
    mappingWiseShortestPaths.clear();
    perInstructionMappingCounter = 0;   // needed in getCurrentMapping()
    for(int i=0; i<inputMappings.size(); i++){
        findShortestPathsFor1InputMapping(); // all SPs for current instruction & current mapping
        mappingWiseShortestPaths.push_back(allSPFSwapPaths);
        perInstructionMappingCounter++; // needed in getCurrentMapping()
    }

}


int QuSmartSwapper::findTotalCostDefaultAllPermutations() {
    unsigned int totalCost = 0;
    init();
    removeUnaryInstructions();
    shared_ptr<QuGate> previousInstruction = nullptr;
    vector<QuMapping> nextInstructionMappings;

    int i=0;
    totalCost = 0;
    programCounter = 0;
    instructionWiseMappings.clear();
    for(shared_ptr<QuGate> instruction: nonUnaryInstructions){
        cout << "CNOT Instruction # " << i++ << endl;
        Util::timeIt(false);
//        Util::setVerbose();
        Util::println("INSTRUCTION ANALYSIS START: " + to_string(programCounter));
        this->currentInstruction = instruction;
//        cout << *instruction << endl;
//        if(currentInstructionSameAsPrevious(previousInstruction, currentInstruction))
//            continue;
        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();

        findShortestPathsForAllInputMappings(inputMappings);
        int before = inputMappings.size();

        auto minData = findMinCostMappingsForNextInstruction(inputMappings);
        auto minCost = minData.first;
        auto filteredSPMappingsForAllMappings = minData.second;
        int after = filteredSPMappingsForAllMappings.size();
        cout << "Before filtering: " << before << endl;
        cout << "Filtered OUT: " << before - after << endl;
        cout << "After filtering: " << after << endl;
        if(!filteredSPMappingsForAllMappings.empty()){
            auto totalMappingsCounter = 0;
            for (auto& mapping: filteredSPMappingsForAllMappings) {
                cout << mapping.toString() << endl;
                mapping.setMappingId(to_string(programCounter) + "." + to_string(totalMappingsCounter)); // todo may be +1
                totalMappingsCounter++;
            }
//            instructionWiseMappings.push_back(inputMappings);
            instructionWiseMappings.push_back(filteredSPMappingsForAllMappings);
//            cout << "filteredSPMappingsForAllMappings: " << filteredSPMappingsForAllMappings.size() << endl;
        }
        else {
            cout << "WWWWWWWWWWWWHHHHHHHHHHHHHHHYYYYYYYYYYYYYYYYYY!" << endl;
        }
//        doExtraHadamardFiltering(currentInstruction, architecture);
        totalCost += minCost;
//        cout << "mincost: " << minCost << " , totalCost: " << totalCost << endl;
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
//        previousInstruction = currentInstruction;
//        cout << "time per instruction: " << Util::timeIt(true) << endl;
    }
    delete allSPF;
    delete allPairShortestPathFinder;

    return totalCost;
}

int QuSmartSwapper::findTotalCostDefault() {
    unsigned int totalCost = 0;
    init();
    removeUnaryInstructions();

    int i=0;
    for(shared_ptr<QuGate> instruction: nonUnaryInstructions){
        cout << "CNOT Instruction # " << i++ << " of " << nonUnaryInstructions.size();
        cout << ": " << *instruction << endl;
        Util::timeIt(false);
        Util::println("INSTRUCTION ANALYSIS START: " + to_string(programCounter));
        this->currentInstruction = instruction;

        // get input mappings to apply on this instruction
        vector<QuMapping> inputMappings = getAllMappingsForCurrentInstruction();
        // all mappingwise shortest paths calculated. Next, need to find all swap permutations
        findShortestPathsForAllInputMappings(inputMappings);
        int before = inputMappings.size();

        auto minData = findMinCostMappingsForNextInstruction(inputMappings);
        auto minCost = minData.first;
        auto filteredSPMappingsForAllMappings = minData.second;
        int after = filteredSPMappingsForAllMappings.size();
        if(!filteredSPMappingsForAllMappings.empty()){
            auto totalMappingsCounter = 0;
            for (auto& mapping: filteredSPMappingsForAllMappings) {
//                cout << mapping.toString() << endl;
                mapping.setMappingId(to_string(programCounter) + "." + to_string(totalMappingsCounter));
                totalMappingsCounter++;
            }
            instructionWiseMappings.push_back(filteredSPMappingsForAllMappings);
            cout << "before filtering: " <<  before << ", after filtering: " << after << endl;
        }

        totalCost += minCost;
        cout << "cost: " << minCost << " , totalCost: " << totalCost << endl;
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
//        cout << "time per instruction: " << Util::timeIt(true) << endl;
    }
    delete allSPF;
    delete allPairShortestPathFinder;

    return totalCost;
}

//pair<int, QuMapping> QuCircuit::findTotalSwaps() {
//    QuSwapStrategy *strategy = new QuSmartSwapper(*this, quArchitecture);
//
////    int min = INT32_MAX;
////    int minPerm = INT32_MAX;
////    strategy->initInitializerMappingCounter();
////    for (int i = 0; i < 2 ; ++i) {
//    cost = strategy->findTotalSwaps();
////        if (cost < min) {
////            min = cost;
////            minPerm = i;
////        }
////        cout << "cost : " << cost << ", min perm : " << minPerm << endl;
////
////    }
////    QuMapping initialMapping(quArchitecture.getN());
//    QuMapping initialMapping = ((QuSmartSwapper*)strategy)->generateOptimalInstructions();
//    delete strategy;
//
//    return make_pair(cost, initialMapping);
//}


pair<int, QuMapping> QuSmartSwapper::findTotalSwaps() {
    unsigned int totalCost = 0;
    if (DAG_SCHEME){
        totalCost = findTotalCostDAG();
    }
    else { // id default scheme
        if (ALL_PERMUTATIONS_SCHEME) {
            totalCost = findTotalCostDefaultAllPermutations();
        }
        else
            totalCost = findTotalCostDefault();
    }
    QuMapping initialMapping = generateOptimalInstructions();

    // sha
//    initialMapping.printShort();

    return make_pair(totalCost, initialMapping);
}

// find all permutations of the shortest path (swapSequence) and find minimum cost mappings for
pair<vector<QuMapping>, int> QuSmartSwapper::findAllMinCostMappingsFromPermutationsFor1Path(QuMapping& inputMapping, vector<int> swapSequence) {
    vector<QuMapping> mappings;
    vector<QuMapping> filteredMappings;
    vector<int> mappingCosts;
    unsigned int totalMoves = swapSequence.size() - 2;
    vector<int> quBitIndexes = currentInstruction->getArgIndex(); // logical qubit index values
    int src = quBitIndexes[0];
    int dest = quBitIndexes[1];
    int srcMoves = 0;
    int destMoves = 0;
    int cost = 0;
    int minCost = INT_MAX;
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
            cost = 0;
            if (srcSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(srcSeq);
                swapGates.insert(swapGates.begin(), swapGatesTemp.begin(), swapGatesTemp.end());
//                cost += calculateHadamardCost(srcSeq, architecture.getCouplingMap());
            }
            if (destSeq.size()>1) {
                swapGatesTemp = mapping.fixMappings(destSeq);
                swapGates.insert(swapGates.end(), swapGatesTemp.begin(), swapGatesTemp.end());
//                cost += calculateHadamardCost(destSeq, architecture.getCouplingMap());
            }
            vector<int> boundaryPair;
            boundaryPair.push_back(srcSeq[srcSeq.size()-1]);
            boundaryPair.push_back(destSeq[destSeq.size()-1]);
            // sha
            cost += calculateHadamardCost(boundaryPair, architecture.getCouplingMap());

            cost += (totalMoves * 7);
            if(swapGates.size()>0) {
                mapping.setSwapInstructions(swapGates);
                mappings.push_back(mapping); // add new mapping generated by a swap path permutation
                mappingCosts.push_back(cost); // add cost of this mapping to filter out the minimum only
            }
        }
        // find min cost
        vector<int> tempMappingCosts = mappingCosts;
        std::sort(tempMappingCosts.begin(), tempMappingCosts.end());
        minCost = tempMappingCosts[0];
        //

        // sift out min. mappings among all mapping permutations generated by 1 input mapping's shortest path
        for (int i = 0; i < mappings.size(); ++i) {
            QuMapping& mapping = mappings[i];
            if (mapping.hasDuplicateMappings())
                cout << "hello!" << endl;
//            if (mappingCosts[i] == minCost) {
//            cout <<
            if ((mappingCosts[i] - minCost) <= 0) {
//            if ((mappingCosts[i] - minCost) <= Constants::FLEXIBILITY_IN_COST) {
                if(mappingCosts[i] != minCost) {
                    cout << "diff" << endl;
                }
                mappings[i].setParentMappingId(inputMapping.getMappingId());
                filteredMappings.push_back(mappings[i]);
            }
        }

    }
    else {
        QuMapping mapping = inputMapping;
        mapping.setParentMappingId(inputMapping.getMappingId());
        mapping.clearSwapInstructions();
        filteredMappings.push_back(mapping);
        vector<int> boundaryPair;
        boundaryPair.push_back(inputMapping.getPhysicalBit(src));
        boundaryPair.push_back(inputMapping.getPhysicalBit(dest));
        // sha
//        minCost = 0;
        minCost = calculateHadamardCost(boundaryPair, architecture.getCouplingMap());
    }

//    if(mappings.empty()) { // no swap happened so copy it after removing swaps
//        QuMapping mapping = inputMapping;
//        mapping.clearSwapInstructions();
//        mappings.push_back(mapping);
//    }
//    cout << endl << endl;
    return make_pair(filteredMappings, minCost);
}


//
//bool QuSmartSwapper::pred(const shared_ptr<QuGate> a, const shared_ptr<QuGate> b){
//    return ((a->getArgAtIndex(0) == b->getArgAtIndex(0)) && (a->getArgAtIndex(1) == b->getArgAtIndex(1))
//         || (a->getArgAtIndex(0) == b->getArgAtIndex(1)) && (a->getArgAtIndex(1) == b->getArgAtIndex(0)));
//}

bool QuSmartSwapper::isNewInsturction(shared_ptr<QuGate> currentInstruction, vector<shared_ptr<QuGate>>& instructions) {
    for (shared_ptr<QuGate> oldInstruction: instructions) {
        if ((*oldInstruction) == (*currentInstruction))
            return false;
    }
    return true;
}




//vector<QuMapping> QuSmartSwapper::generateInitialMappings() {
//    if (ALL_PERMUTATIONS_SCHEME) {
//        initialMappings = mappingInitializer.getNextPermutationMapping();
//    }
//    else {
//    //    mappingInitializer.initGenerator(architecture.getN());
//        vector<pair<int, int>> restrictionList;
//
//        vector<pair<int, int>> restrictionListSources;
//
//        // set k unique instructions to restrict // todo SHA: no restrictions
//        if (INIT_MAPPING_RESTRICT_MODE) {
//            pair<vector<pair<int, int>>, vector<pair<int, int>>> restrictionData = makeRestrictionPairList(K);
//            restrictionList = restrictionData.first;
//            restrictionListSources = restrictionData.second;
//        }
//        //    vector<pair<int, int>> restrictionList = makeRestrictionPairList(architecture.getN()/2);
//        initialMappings = mappingInitializer.generateSmartMappings(restrictionListSources, restrictionList,architecture);
//    }
////    initialMappings = mappingInitializer.generateSmartMappings(restrictionListSources, restrictionList,architecture);
//
////    Zulehner
////    QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
////    vector<int> instructionIds;
////    auto layerMap = layerManager->buildLayerwiseInstructionsMap();
////    QuCircuitLayerManager::deleteInstance();
////
////    layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
////
////    instructionIds = layerManager->getNextSourceInstructionIds();
////    nonUnaryInstructionsMap = layerManager->getInstructionMap(); // to get instruction objects
////
////    vector<shared_ptr<QuGate>> layer0Instructions;
////    for (auto id: instructionIds) {
////        layer0Instructions.push_back(nonUnaryInstructionsMap[id]);
////    }
////
////    for (int i = 0; i < layer0Instructions.size(); ++i) {
////        shared_ptr<QuGate> gate = layer0Instructions[i];
////        cout << *gate << endl;
////        pair<int, int> newPair = make_pair(gate->getArgAtIndex(0), gate->getArgAtIndex(1));
////        restrictionList.push_back(newPair);
////    }
//
////    mappingInitializer.setLayer0Instructions(layer0Instructions);
////    initialMappings = mappingInitializer.generateSmartMappings(restrictionList, architecture);
//    return initialMappings;
//}

vector<QuMapping> QuSmartSwapper::pruneMappings(bool random) {

}

vector<QuMapping> QuSmartSwapper::getAllMappingsForCurrentInstruction() {
    vector<QuMapping> mappings;
    string pid = "*";
    if (!programCounter) { // 1st instruction
            mappings = initialMappings;
    }
    else
        mappings = instructionWiseMappings[programCounter-1];
    if(mappings.size() > MAPPING_THRESHOLD) {
        cout << "MAPPINGS FOR NEXT ITERATION (BEFORE PRUNING): " << mappings.size() << endl;

        if (RANDOM_SAMPLING_MAPPINGS_PRUNING) {
            // todo divide threshold among mappings
            vector<QuMapping> sampledMappings;
            unsigned long threshold = MAPPING_THRESHOLD;
            if (mappings.size() <= threshold)
                threshold = mappings.size();
            Util::randomSampling(mappings, sampledMappings, threshold);
            mappings = sampledMappings;
        }
        else {
            mappings.erase(mappings.begin() + MAPPING_THRESHOLD, mappings.end());
        }
//        cout << "MAPPINGS FOR NEXT ITERATION (AFTER PRUNING): " << mappings.size() << endl;
    }
    else
        ;
//        cout << "MAPPINGS FOR NEXT ITERATION (NO PRUNING): " << mappings.size() << endl;

return mappings;
}

QuMapping QuSmartSwapper::getCurrentMapping() {
    QuMapping currentMapping(this->architecture.getN());
    if(!programCounter)
        if (ALL_PERMUTATIONS_SCHEME)
            currentMapping = initialMappings[0];
        else
            currentMapping = initialMappings[perInstructionMappingCounter];
    else
        currentMapping = instructionWiseMappings[programCounter-1][perInstructionMappingCounter];
    return currentMapping;
}

// returns the minimum total cost of all shortest paths before permutations
void QuSmartSwapper::findShortestPathsFor1InputMapping() {
    vector<int> shortestPath;
//    int edgeDistance = 0;
    swaps = 0;
    swapPath.clear();   // for 1 SP
    allSPFSwapPaths.clear();    // for all SPs

    QuMapping mapping = getCurrentMapping();
    if (mapping.getMappingId()=="88.0")
        cout << mapping.isLegit() << " " << mapping.hasDuplicateMappings() << endl;
    int src = mapping.getPhysicalBit(currentInstruction->getArgAtIndex(0));
    int dest = mapping.getPhysicalBit(currentInstruction->getArgAtIndex(1));

    // returns the shortest path and the cost as a pair
    pair<vector<int>, int> result = allPairShortestPathFinder->getSPSequence(src, dest);
    shortestPath = result.first;
//    edgeDistance = result.second; // # of swaps + 1 OR # of hops OR edge distance
    swaps = shortestPath.size() - 2;
    if (swaps > 0) { // find all shortest paths
        allSPFSwapPaths = allSPF->findSingleSourceAllShortestPaths(src, dest, swaps); // todo find shortest w/o swaps arg

    }
    else
        allSPFSwapPaths.push_back(shortestPath);
}

vector<int> QuSmartSwapper::swapAlongPath(int* parent, int source, int destination) {
//    int count = 0;

    if (parent[destination] != -1){
        swapAlongPath(parent, source, parent[destination]);
        swapPath.push_back(destination);
        // insert swap gate in circuit
//        insertSwapGates(source, destination);  // todo swap gates inserted in 1 sequence fro src to dest. must be swapPath dependent
//        shared_ptr<QuGate> swapGate = QuGateFactory::getQuGate("SWAP");    // create a new swap gate
//        int* args = swapGate -> getArgIndex();
//        args[0] = getCurrentMapping().getLogicalMapping(source);       // set swap gate 1st arg
//        args[1] = getCurrentMapping().getLogicalMapping(destination);  // set swap gate 2nd arg
//
//        circuit.getInstructions1().push_back(swapGate);               // add swap gate to circuit
    }
    return swapPath;
}

void QuSmartSwapper::insertSwapGates(int source, int destination){
        // insert swap gate in circuit
    shared_ptr<QuGate> swapGate = QuGateFactory::getQuGate("SWAP");    // create a new swap gate
    vector<int> args = swapGate -> getArgIndex();
    swapGate -> setArgAtIndex(0, getCurrentMapping()[source]);       // set swap gate 1st arg
    swapGate -> setArgAtIndex(1, getCurrentMapping()[destination]);       // set swap gate 2nd arg
//    swapGate -> setArgAtIndex(0, getCurrentMapping().getLogicalMapping(source));       // set swap gate 1st arg
//    swapGate -> setArgAtIndex(1, getCurrentMapping().getLogicalMapping(destination));       // set swap gate 2nd arg
    circuit.getInstructions1().push_back(swapGate);               // add swap gate to circuit
}

QuSmartSwapper::QuSmartSwapper(QuCircuit &circuit, QuArchitecture& architecture, vector<QuMapping>& initialMappings)
        : QuSwapStrategy(circuit, architecture), perInstructionMappingCounter(0), allSPF(
        nullptr), allPairShortestPathFinder(nullptr), hadamards(0), initialMappings(initialMappings) {
    for (int i = 0; i < initialMappings.size(); ++i) {
        initialMappings[i].setParentMappingId("*");
        initialMappings[i].setMappingId("0." + to_string(i));
    }

}

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


QuMapping QuSmartSwapper::generateOptimalInstructionsDefault() {
    int nonUnarySize = instructionWiseMappings.size() - 1;
    string parentMappingId;
    vector<shared_ptr<QuGate>> finalProgram;
    int parentProgramCounter, parentMappingCounter;
    hadamards = 0;
    QuMapping theMapping(architecture.getN()); // the initial mapping selected for program gen.
    if (!nonUnaryInstructions.empty()){
        theMapping = instructionWiseMappings[nonUnarySize][0]; // get any (1st) last mapping to start backtracking
        vector<QuMapping> selectedMappings;

        int i;
        for (i = nonUnarySize - 1; i >= 0; i--) {
            selectedMappings.insert(selectedMappings.begin(), theMapping);
            parentMappingId = theMapping.getParentMappingId();
            Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
            theMapping = instructionWiseMappings[parentProgramCounter][parentMappingCounter];
            Util::println("B: theMapping.getMappingId(): " + theMapping.getMappingId());
            Util::println("B: theMapping.getParentMappingId(): " + theMapping.getParentMappingId());
        }
        selectedMappings.insert(selectedMappings.begin(), theMapping);

        int x = 0; // x : index of next non-Unary instruction
        swaps = 0;
        for (i = 0; i < selectedMappings.size(); i++) {
//            selectedMappings[i].print();
            x = insertRemovedUnaryInstructions(finalProgram, x);
            vector<shared_ptr<QuGate>> swapInstructions;
            if(!ELEMENTARY_SWAP_DECOMPOSITION) { // swap gates NOT decomposed into elementary
                for (int j = 0; j < selectedMappings[i].getSwapInstructions().size(); j++) {
                    shared_ptr<QuGate> swapGate = QuGateFactory::getQuGate("SWAP");
                    int src = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0];
                    int target = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1];
                    swapGate->setArgAtIndex(0, src);
                    swapGate->setArgAtIndex(1, target);
                    swapInstructions.push_back(swapGate);
                    swaps++;
                }
                for (int i = 0; i < swapInstructions.size(); i++) {
                    finalProgram.push_back(swapInstructions[i]);
                }
            }
            else { // swap gates decomposed into elementary
                shared_ptr<QuGate> cnot = nullptr;
                shared_ptr<QuGate> h = nullptr;

                for (int j = 0; j < selectedMappings[i].getSwapInstructions().size(); j++) {
                    int src = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0];
                    int target = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1];
//                    cx q[1], q[2];
//                    h q[1];
//                    h q[2];
//                    cx q[1], q[2];
//                    h q[1];
//                    h q[2];
//                    cx q[1], q[2];
                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, src);
                    swapInstructions.push_back(h);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, target);
                    swapInstructions.push_back(h);

                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, src);
                    swapInstructions.push_back(h);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, target);
                    swapInstructions.push_back(h);

                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    swaps++;
                }
                for (int i = 0; i < swapInstructions.size(); i++) {
                    finalProgram.push_back(swapInstructions[i]);
                }
            }
            hadamardCheck(finalProgram, architecture, selectedMappings[i], i);

        }
    }

    insertEndingUnaryInstructions(finalProgram);
//    for (int k = 0; k < finalProgram.size(); ++k) {
//        cout << *finalProgram[k] << endl;
//    }
    circuit.setOptimizations(optimize(finalProgram));

    circuit.setInstructions1(finalProgram);
    circuit.setSwaps(swaps);  // todo  should it be swaps??
    circuit.setHadamards(hadamards);


    return theMapping;
}


QuMapping QuSmartSwapper::generateOptimalInstructionsDAG() {
    int nonUnarySize = instructionWiseMappings.size() - 1;
    string parentMappingId;
    vector<shared_ptr<QuGate>> finalProgram;
    int parentProgramCounter, parentMappingCounter;
    hadamards = 0;
    QuMapping theMapping(architecture.getN()); // the initial mapping selected for program gen.

//    QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
//    selectedNonUnaryInstructionIds
    if (!nonUnaryInstructions.empty()){  // todo SHA: use DAG sequence
        theMapping = instructionWiseMappings[nonUnarySize][0]; // get any (1st) last mapping to start backtracking
        vector<QuMapping> selectedMappings;

        int i;
        // prepare hierarchical list of mappings that will lead to the optimized code
        for (i = nonUnarySize - 1; i >= 0; i--) { // todo fix dag mapping backtrack issue0-
            selectedMappings.insert(selectedMappings.begin(), theMapping);
            parentMappingId = theMapping.getParentMappingId();
            Util::parseMappingId(parentMappingId, parentProgramCounter, parentMappingCounter);
            theMapping = instructionWiseMappings[parentProgramCounter - 1][parentMappingCounter];
            Util::println("B: theMapping.getMappingId(): " + theMapping.getMappingId());
            Util::println("B: theMapping.getParentMappingId(): " + theMapping.getParentMappingId());
        }
        selectedMappings.insert(selectedMappings.begin(), theMapping);

        int x = 0; // x : index of next non-Unary instruction
        swaps = 0;
        for (i = 0; i < selectedMappings.size(); i++) {
//            selectedMappings[i].print();
            x = insertRemovedUnaryInstructions(finalProgram, x);
            vector<shared_ptr<QuGate>> swapInstructions;
            if(!ELEMENTARY_SWAP_DECOMPOSITION) { // swap gates NOT decomposed into elementary
                for (int j = 0; j < selectedMappings[i].getSwapInstructions().size(); j++) {
                        shared_ptr<QuGate> swapGate = QuGateFactory::getQuGate("SWAP");
                        int src = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0];
                        int target = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1];
                        swapGate->setArgAtIndex(0, src);
                        swapGate->setArgAtIndex(1, target);
                        swapInstructions.push_back(swapGate);
                        swaps++;
                }
                for (int i = 0; i < swapInstructions.size(); i++) {
                    finalProgram.push_back(swapInstructions[i]);
                }
            }
            else { // swap gates decomposed into elementary
                shared_ptr<QuGate> cnot = nullptr;
                shared_ptr<QuGate> h = nullptr;

                for (int j = 0; j < selectedMappings[i].getSwapInstructions().size(); j++) {
                    int src = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[0];
                    int target = selectedMappings[i].getSwapInstructions()[j].getArgIndex()[1];
//                    cx q[1], q[2];
//                    h q[1];
//                    h q[2];
//                    cx q[1], q[2];
//                    h q[1];
//                    h q[2];
//                    cx q[1], q[2];
                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, src);
                    swapInstructions.push_back(h);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, target);
                    swapInstructions.push_back(h);

                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, src);
                    swapInstructions.push_back(h);

                    h = QuGateFactory::getQuGate("h");
                    h->setArgAtIndex(0, target);
                    swapInstructions.push_back(h);

                    cnot = QuGateFactory::getQuGate("cx");
                    cnot->setArgAtIndex(0, src);
                    cnot->setArgAtIndex(1, target);
                    swapInstructions.push_back(cnot);

                    swaps++;
                }
                for (int i = 0; i < swapInstructions.size(); i++) {
                    finalProgram.push_back(swapInstructions[i]);
                }
            }
            hadamardCheck(finalProgram, architecture, selectedMappings[i], i);

        }
    }

    insertEndingUnaryInstructions(finalProgram);

    circuit.setOptimizations(optimize(finalProgram));

    circuit.setInstructions1(finalProgram);
    circuit.setSwaps(swaps);  // todo  should it be swaps??
    circuit.setHadamards(hadamards);


    return theMapping;
}


QuMapping QuSmartSwapper::generateOptimalInstructions() {
    QuMapping theMapping(architecture.getN()); // the initial mapping selected for program gen.
    if (DAG_SCHEME)
        theMapping = generateOptimalInstructionsDAG();
    else
        theMapping = generateOptimalInstructionsDefault();

    return theMapping;
}


int QuSmartSwapper::optimize(vector<shared_ptr<QuGate>>& finalProgram){
    int physicalBits = architecture.getN();
    int cnotCancellations = QuCircuitOptimizer::performCNOTCancellations(physicalBits, finalProgram);
    cout << "CNOT cancellations: " << cnotCancellations << endl;
    int unaryCancellations = QuCircuitOptimizer::performUnaryCancellations(physicalBits, finalProgram);
    cout << "Unary cancellations: " << unaryCancellations << endl;
    cout << "Total optimization: " << unaryCancellations + cnotCancellations << endl;
    return unaryCancellations + cnotCancellations;
}

void QuSmartSwapper::insertEndingUnaryInstructions(vector<shared_ptr<QuGate>>& finalProgram) {
    vector<shared_ptr<QuGate>> originalProgram = circuit.getInstructions0();
    int i=originalProgram.size()-1;
    while(i>=0 && originalProgram[i]->isUnary()){
        i--;
//        cout << "unary" << endl;
    }
    if (i!=originalProgram.size()-1)
        finalProgram.insert(finalProgram.end(), originalProgram.begin()+i+1, originalProgram.end());
}

int QuSmartSwapper::insertRemovedUnaryInstructions(vector<shared_ptr<QuGate>>& finalProgram, int nextNonUnaryIndex) {
    int index = 0;
    if (DAG_SCHEME){
        index = insertRemovedUnaryInstructionsDAG(finalProgram, nextNonUnaryIndex);
    }
    else {
        index = insertRemovedUnaryInstructionsDefault(finalProgram, nextNonUnaryIndex);
    }
    return index;
}

int QuSmartSwapper::insertRemovedUnaryInstructionsDefault(vector<shared_ptr<QuGate>>& finalProgram, int nextNonUnaryIndex) {
    vector<shared_ptr<QuGate>> originalProgram = circuit.getInstructions0();
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

// todo
int QuSmartSwapper::insertRemovedUnaryInstructionsDAG(vector<shared_ptr<QuGate>>& finalProgram, int nextNonUnaryIndex) {
    vector<shared_ptr<QuGate>> originalProgram = circuit.getInstructions0();
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

void QuSmartSwapper::hadamardCheck(vector<shared_ptr<QuGate> > &finalProgram, QuArchitecture &quArchitecture, QuMapping &currentMapping,
                              int index) {
    vector<shared_ptr<QuGate>> instructions;
    shared_ptr<QuGate> currentInstruction = nullptr;
    if (DAG_SCHEME) {
        for (auto id: selectedNonUnaryInstructionIds) {
            instructions.push_back(nonUnaryInstructionsMap[id]);
        }
        currentInstruction = instructions[index];
    }
    else {
        currentInstruction = nonUnaryInstructions[index];
    }
//    shared_ptr<QuGate> currentInstruction = nonUnaryInstructions[index];

    int src = currentInstruction->getArgAtIndex(0);
    int target = currentInstruction->getArgAtIndex(1);
    int physicalSrc = currentMapping.getPhysicalBit(src);
    int physicalTarget = currentMapping.getPhysicalBit(target);
    shared_ptr<QuGate> hadamard = NULL;
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

void QuSmartSwapper::mappingSanityCheck() {
    int nonUnarySize = instructionWiseMappings.size()-1;
    string parentMappingId;
    int parentProgramCounter, parentMappingCounter;


    vector<QuMapping> selectedMappings;

    Util::println("C: theMapping.getMappingId(): " + instructionWiseMappings[0][0].getMappingId());
    Util::println("C: theMapping.getParentMappingId(): " + instructionWiseMappings[0][0].getParentMappingId());

    QuMapping theMapping(this->architecture.getN());

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

pair<int, vector<QuMapping>> QuSmartSwapper::findMinCostMappingsForNextInstruction(vector<QuMapping> &inputMappings) {
    vector<QuMapping> filteredSPMappingsForAllMappings;
    vector<int> minCostForAllMappings;
    vector<vector<QuMapping>> minMappingsOfAllInputMappings;
    unsigned int absoluteMin = 0;
    // for each mapping for each shortest path find min permutation-cost mappings
    for (unsigned int j = 0; j < inputMappings.size(); j++) {
//        inputMappings[j].clearSwapInstructions(); todo: is this required??
        vector<QuMapping> minMappingsFrom1Mapping;
        vector<int> minCostForAllPaths;
        vector<vector<QuMapping>> minMappingsForAllPaths;

        // sift out min. mappings among all mappings generated by all shortest paths of 1 input mapping
        for (unsigned int k = 0; k < mappingWiseShortestPaths[j].size(); k++) {
            // sift out min. mappings among all mappings generated by finding all permutations of 1 path of 1 input mapping
            auto permMappingsStuff = findAllMinCostMappingsFromPermutationsFor1Path(inputMappings[j],mappingWiseShortestPaths[j][k]);
            auto minMappingsFor1Path = permMappingsStuff.first;
            auto minCostFor1Path = permMappingsStuff.second;
            minMappingsForAllPaths.push_back(minMappingsFor1Path);
            minCostForAllPaths.push_back(minCostFor1Path);;
        }

        // sift out min. mappings among all mappings generated by 1 input mapping
        auto minCostFor1Mapping = Util::findMin(minCostForAllPaths);
        for (unsigned int k = 0; k < mappingWiseShortestPaths[j].size(); k++) {
//            if (minCostForAllPaths[k] == minCostFor1Mapping) {
//            cout << "(minCostForAllPaths[k] - minCostFor1Mapping) : " << (minCostForAllPaths[k] - minCostFor1Mapping) <<endl;
//            if ((minCostForAllPaths[k] - minCostFor1Mapping) <= Constants::FLEXIBILITY_IN_COST) {
            if ((minCostForAllPaths[k] - minCostFor1Mapping) <= 0) {
                if(minCostForAllPaths[k] != minCostFor1Mapping)
                    cout << "diff" << endl;
                minMappingsFrom1Mapping.insert(minMappingsFrom1Mapping.end(), minMappingsForAllPaths[k].begin(), minMappingsForAllPaths[k].end());
            }
        }
        minCostForAllMappings.push_back(minCostFor1Mapping);
        minMappingsOfAllInputMappings.push_back(minMappingsFrom1Mapping);
    }

    // sift out min. mappings among all mappings generated by all input mappings
    absoluteMin = Util::findMin(minCostForAllMappings); // todo: think
    for (unsigned int k = 0; k < minMappingsOfAllInputMappings.size(); k++) {
        if (minCostForAllMappings[k] == absoluteMin) {
//        if ((minCostForAllMappings[k] - absoluteMin) <= Constants::FLEXIBILITY_IN_COST) {
            filteredSPMappingsForAllMappings.insert(filteredSPMappingsForAllMappings.end(), minMappingsOfAllInputMappings[k].begin(), minMappingsOfAllInputMappings[k].end());
        }
    }
//    int x = absoluteMin ? Constants::FLEXIBILITY_IN_COST : 0;
//    int x = Constants::FLEXIBILITY_IN_COST;
//    return make_pair(absoluteMin + x, filteredSPMappingsForAllMappings);
    return make_pair(absoluteMin, filteredSPMappingsForAllMappings);
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

bool QuSmartSwapper::currentInstructionSameAsPrevious(shared_ptr<QuGate> previous, shared_ptr<QuGate> current) {
    if (previous != nullptr && (previous->isDitto(currentInstruction))) {
        instructionWiseMappings.push_back(instructionWiseMappings[instructionWiseMappings.size()-1]);
        updateMappingIdsForDitto();
        Util::println("INSTRUCTION ANALYSIS END: " + to_string(programCounter));
        programCounter++;
        return true;
    }
    return false;
}

void QuSmartSwapper::doExtraHadamardFiltering(shared_ptr<QuGate> currentInstruction, QuArchitecture &quArchitecture) {
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

