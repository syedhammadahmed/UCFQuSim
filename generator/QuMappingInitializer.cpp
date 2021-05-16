//
// Created by SHA on 7/24/2020.
//

#include "util/Util.h"
#include <queue>
#include <algorithm>
#include <core/QuCircuit.h>
#include <util/Constants.h>
#include "Config.h"
#include "QuMappingInitializer.h"
#include "PriorityGraph.h"
#include "QuCircuitLayerManager.h"

using namespace std;

//int QuMappingInitializer::TOTAL_PERM = 1000000000; // # of qubits
//int QuMappingInitializer::TOTAL_PERM = 1000; // # of qubits
//int QuMappingInitializer::n = 0; // # of qubits
//int QuMappingInitializer::count = 0; // permutation count
//QuMapping QuMappingInitializer::defaultMapping;
//vector<string> QuMappingInitializer::perms;



void QuMappingInitializer::generatePermutations() {
    vector<int> permInput;
    int n = architecture.getN();
    for (int i = 0; i < n; ++i) {
        permInput.push_back(i);
    }
    if (permInput.size() > PERM_N) // 10! perms
        permInput.erase(permInput.begin() + PERM_N, permInput.end());
    Util::permute(permInput, 0, permInput.size() - 1, perms);
    cout << "Permutations generated!"<< endl;
    if (perms.size() > TOTAL_PERM) {
        perms.erase(perms.begin() + TOTAL_PERM, perms.end());
    }
}

void QuMappingInitializer::generateMappingsFromPermutations() {
    initialMappings.clear();
    for (auto& perm: perms) {
        QuMapping mapping(architecture.getN());
        mapping.init(perm);
        initialMappings.push_back(mapping);
    }
}


//void QuMappingInitializer::generateMappings() {
//    string str = "0123456789";
//    int n = str.size();
//    Util::permute(str, 0, n-1, QuMappingInitializer::perms);
//}
//
//void QuMappingInitializer::makeSmartCouples(architecture& architecture){
//    couplingMapAdjList.resize(architecture.getN());
//
//    for(int i = 0; i < architecture.getN(); i++) {
//        for (int j = 0; j < architecture.getN(); j++){
//            if ((i != j) && (architecture.getCouplingMap()[i][j] > 0)){
//                 couples.push_back(make_pair(i, j));
//                couplingMapAdjList[i].push_back(j);
//                couplingMapAdjList[j].push_back(i);
//            }
//        }
//    }
// make couples according to src freq priority
//    vector<pair<int, int>> srcPriorityListPhysical = architecture.getSrcFreqPriorityList();
//    vector<pair<int, int>> targetPriorityListPhysical = architecture.getTargetFreqPriorityList();
//
//    for (const auto& pair: srcPriorityListPhysical) { // make couples according to src freq priority
//        vector<int> neighbors = couplingMapAdjList[pair.first];
//        for (auto& item: neighbors) {
//            couples.push_back(make_pair(pair.first, item));
//        }
//    }
// make couples according to target freq priority todo: bad bad bad remove it
//    vector<pair<int, int>> srcPriorityListPhysical = architecture.getSrcFreqPriorityList();
//    vector<pair<int, int>> targetPriorityListPhysical = architecture.getTargetFreqPriorityList();
//
//    for (const auto& pair: targetPriorityListPhysical) { // make couples according to src freq priority
//        vector<int> neighbors = couplingMapAdjList[pair.second];
//        for (auto& item: neighbors) {
//            couples.push_back(make_pair(item, pair.second));
//        }
//    }
//}

void QuMappingInitializer::makeCouples(){
    couplingMapAdjList.resize(architecture.getN());
    for(int i = 0; i < architecture.getN(); i++) {
        for (int j = 0; j < architecture.getN(); j++){
            if ((i != j) && (architecture.getCouplingMap()[i][j] > 0)){
//                counts[i].second++;
                couples.push_back(make_pair(i, j));
                if(IGNORE_DIRECTION_IN_ZERO_COST_INIT_MAPPING_RESTRICTION)
                    couples.push_back(make_pair(j, i));
                couplingMapAdjList[i].push_back(j);
                couplingMapAdjList[j].push_back(i);
            }
        }
    }
//    Util::printPairs(couples);
}

void QuMappingInitializer::makeCouplingMapAdjacencyList(){
    couplingMapAdjList.resize(architecture.getN());
    for(int i = 0; i < architecture.getN(); i++) {
        for (int j = 0; j < architecture.getN(); j++){
            if ((i != j) && (architecture.getCouplingMap()[i][j] > 0)){
                couplingMapAdjList[i].push_back(j);
                if(IGNORE_DIRECTION_IN_ZERO_COST_INIT_MAPPING_RESTRICTION)
                    couplingMapAdjList[j].push_back(i);
            }
        }
    }
//    Util::printPairs(couples);
}

struct MappingEqualityComparator {
    bool pred(QuMapping &a, QuMapping &b) {
        for (int i = 0; i < a.getN(); ++i) {
            if (a.getValueAt(i) != b.getValueAt(i))
                return false;
        }
        return true;
    }
};

//vector<QuMapping> QuMappingInitializer::generateSmartMappings(vector<pair<int, int>> restrictionListSources, vector<pair<int, int>> restrictionPairs, architecture& architecture) {
//    vector<QuMapping> initMappings;
//
//    architecture.makeSourceFrequencyPriorityList();
//    architecture.makeTargetFrequencyPriorityList();
//    architecture.makeCommonFrequencyPriorityLists();
//
//    makeCouples(architecture);  // makes an adj list of coupling map for restriction mappings
//
//// remove restricted qubits from perm input string
////    string str = "0123456789";
////    int n = str.size();
////    for(int i=0; i<restrictionPairs.size(); i++){
////        int pos1 = str.find(to_string(restrictionPairs[i].first));
////        if (pos1 != -1) str.replace(pos1, 1, "");
////        pos1 = str.find(to_string(restrictionPairs[i].second));
////        if (pos1 != -1) str.replace(pos1, 1, "");
////
////        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
////    }
//// remove restricted qubits from perm input vector
//    vector<int> permInput;
//    for (int i = 0; i < n; ++i) {
//        permInput.push_back(i);
//    }
//
//    // testing.. begin todo remove this and uncomment line that follows the block
//
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[0].first), permInput.end());
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[0].second), permInput.end());
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[1].first), permInput.end());
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictio1000nPairs[1].second), permInput.end());
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[2].first), permInput.end());
////    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[2].second), permInput.end());
////
////    restrictedMapping.setValueAt(3, 0);
////    restrictedMapping.setValueAt(2, 6);
////    restrictedMapping.setValueAt(14, 5);
//
//    // testing.. end
//
//    for(auto& pair: restrictionListSources) {
//        rankGraph.caterNode(pair.first, true);
//        rankGraph.caterNode(pair.second, false);
//    }
//    rankGraph.sortByRank();
//    commonSrcListPhysical = architecture.getCommonSrcFreqPriorityList();
//    commonTargetListPhysical = architecture.getCommonTargetFreqPriorityList();
//    srcListPhysical = architecture.getSrcFreqPriorityList();
//    targetListPhysical = architecture.getTargetFreqPriorityList();
//
//    for(int i=0; i<rankGraph.getSize(); i++){   // todo
//        int physicalQuBit = -1;
//        PriorityNode logicalQuBitToAllocate = rankGraph.getHead();  // top (i+1)-th node and deleted on returning
//
//        if (i==0) {  // just assigning 1 logical qubit
//            permInput.erase(remove(permInput.begin(), permInput.end(), logicalQuBitToAllocate.getLogicalQubit()), permInput.end());
//            if (logicalQuBitToAllocate.getOutDegree() > logicalQuBitToAllocate.getInDegree()) {
//                if (!commonSrcListPhysical.empty()) {
//                    physicalQuBit = commonSrcListPhysical[0].first;
//                    commonSrcListPhysical.erase(commonSrcListPhysical.begin());
//                } else {
//                    physicalQuBit = srcListPhysical[0].first;
//                    srcListPhysical.erase(srcListPhysical.begin());
//                }
//            } else if (logicalQuBitToAllocate.getOutDegree() <= logicalQuBitToAllocate.getInDegree()) {
//                if (!commonTargetListPhysical.empty()) {
//                    physicalQuBit = commonTargetListPhysical[0].first;
//                    commonTargetListPhysical.erase(commonTargetListPhysical.begin());
//                } else {
//                    physicalQuBit = targetListPhysical[0].first;
//                    targetListPhysical.erase(targetListPhysical.begin());
//                }
//            }
//            restrictedMapping.setValueAt(physicalQuBit, logicalQuBitToAllocate.getLogicalQubit());
//            allocated[logicalQuBitToAllocate.getLogicalQubit()] = true;
//        }
////        else{
////            if (isAdjacent)
////        }
//
////        smartRestrict(restrictionPairs[i].first, restrictionPairs[i].second);
//    }
//    // todo remove next 2 lines after testing
////    restrictionPairs.clear();
////    restrictedMapping.strongInit();
//
//    for(int i=0; i<restrictionPairs.size(); i++){
//        if (!permInput.empty()) {
//            permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
//            permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
//        }
//        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
//    }
//
////
//    //    for(int i=0; i<restrictionPairs.size(); i++){
////        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
////        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
////
////        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
////    }
//
//    if (permInput.size()>10) // 10! perms
//        permInput.erase(permInput.begin()+10, permInput.end());
//    Util::permute(permInput, 0, permInput.size()-1, perms);
//
//    if (perms.size() > TOTAL_PERM) { // todo random sampling - DONE
//        perms.erase(perms.begin() + TOTAL_PERM - 1, perms.end());
//    }
//    TOTAL_PERM = perms.size();
//
//                  //    perms = Util::getNRandomPermutations(TOTAL_PERM, permInput);  // todo uncomment for sampling
//
//    for(int i=0; i<TOTAL_PERM; i++){
//        initMappings.push_back(getNextMapping());
//    }
//    // todo remove duplicate mappings
////    std::unique(initMappings.begin(), initMappings.end(), MappingEqualityComparator());
//
//
//    initMappings.clear(); // todo remove it.. just testing..
//    QuMapping initialMapping;
//    initialMapping.defaultInit();
////    initialMapping.setParentMappingId("*");
////    initialMapping.setMappingId("0." + to_string(count)); //temp[i].setMappingId(to_string(programCounter) + "." + to_string(i));
////    initMappings.clear();
//    initMappings.insert(initMappings.begin(), initialMapping);
//
//    // remove duplicate initial mappings
//
//    auto it = std::unique (initMappings.begin(), initMappings.end());   // (no changes)
//    initMappings.resize( std::distance(initMappings.begin(),it) );
//
//    return initMappings;
//}

PriorityNode QuMappingInitializer::allocateTopRankNode(){
    int physicalQuBit = -1;
    vector<pair<int, int>> commonSrcListPhysical = architecture.getCommonSrcFreqPriorityList();
    vector<pair<int, int>> commonTargetListPhysical = architecture.getCommonTargetFreqPriorityList();
    vector<pair<int, int>> targetListPhysical = architecture.getTargetFreqPriorityList();


    PriorityNode logicalQuBitToAllocate = rankGraph.getHead();  // top (i+1)-th node and deleted on returning
    vector<pair<int, int>> srcListPhysical = architecture.getSrcFreqPriorityList();
    permInput.erase(remove(permInput.begin(), permInput.end(), logicalQuBitToAllocate.getLogicalQubit()), permInput.end());
    if (logicalQuBitToAllocate.getOutDegree() > logicalQuBitToAllocate.getInDegree()) {
        if (!commonSrcListPhysical.empty()) {
            physicalQuBit = commonSrcListPhysical[0].first;
            commonSrcListPhysical.erase(commonSrcListPhysical.begin());
        } else {
            physicalQuBit = srcListPhysical[0].first;
            srcListPhysical.erase(srcListPhysical.begin());
        }
    } else if (logicalQuBitToAllocate.getOutDegree() <= logicalQuBitToAllocate.getInDegree()) {
        if (!commonTargetListPhysical.empty()) {
            physicalQuBit = commonTargetListPhysical[0].first;
            commonTargetListPhysical.erase(commonTargetListPhysical.begin());
        } else {
            physicalQuBit = targetListPhysical[0].first;
            targetListPhysical.erase(targetListPhysical.begin());
        }
    }
    restrictedMapping.setValueAt(physicalQuBit, logicalQuBitToAllocate.getLogicalQubit());
    allocated[logicalQuBitToAllocate.getLogicalQubit()] = true;
    logicalQuBitToAllocate.setPhysicalQubit(physicalQuBit);

    return logicalQuBitToAllocate;
}

//
//vector<QuMapping> QuMappingInitializer::generateHardcodedMappings(vector<int> physicalToLogical, architecture& architecture) {
//    vector<QuMapping> initMappings;
//    QuMapping initMapping(architecture.getN());
//    initMapping.setPhysicalToLogical(physicalToLogical);
//    initMappings.push_back(initMapping);
//
//    return initMappings;
//}

vector<QuMapping> QuMappingInitializer::generateSmartMappings(vector<pair<int, int>> restrictionListSources, vector<pair<int, int>> restrictionPairs) {
    vector<QuMapping> initMappings;
    int n = architecture.getN();
    if (INIT_MAPPING_DEFAULT_ONLY || INIT_MAPPING_HARD_CODED_ONLY) {
        initMappings.clear(); // todo remove it.. just testing..
        QuMapping initialMapping(n);
        if (INIT_MAPPING_DEFAULT_ONLY)
            initialMapping.defaultInit();
        else
            initialMapping.hardCodedInit();

        initialMapping.setParentMappingId("*");
        initialMapping.setMappingId("0.0");

        initMappings.insert(initMappings.begin(), initialMapping);
    }
    else {
        if (INIT_MAPPING_START_NODE_RANK_WISE) {
            buildRankGraph(restrictionListSources);
            startingNode = allocateTopRankNode(); // starting point of init mapping allocation
            makeCouples();  // makes an adj list of coupling map for restriction mappings
        }
        else
            makeCouples();

        // testing.. begin todo remove this and uncomment line that follows the block

//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[0].first), permInput.end());
//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[0].second), permInput.end());
//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[1].first), permInput.end());
//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictio1000nPairs[1].second), permInput.end());
//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[2].first), permInput.end());
//    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[2].second), permInput.end());
//
//    restrictedMapping.setValueAt(3, 0);
//    restrictedMapping.setValueAt(2, 6);
//    restrictedMapping.setValueAt(14, 5);

        // testing.. end

        // todo remove next 2 lines after testing
//    restrictionPairs.clear();
//    restrictedMapping.noMappingInit();

        if (INIT_MAPPING_RESTRICT_MODE) {
            for (int i = 0; i < restrictionPairs.size(); i++) {
                restrict(restrictionPairs[i].first, restrictionPairs[i].second);
                if (!permInput.empty()) {
                    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
                    permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
                }
            }
        }
//
        //    for(int i=0; i<restrictionPairs.size(); i++){
//        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
//        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
//
//        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
//    }
        int totalPermutations = TOTAL_PERM;

        if (RANDOM_SAMPLING_INIT_MAPPINGS) {
            perms = Util::getNRandomPermutations(totalPermutations, permInput);
        }
        else {
            // no random sampling start
            generatePermutations();
//            if (permInput.size() > PERM_N) // 10! perms
//                permInput.erase(permInput.begin() + PERM_N, permInput.end());
//            Util::permute(permInput, 0, permInput.size() - 1, perms);
//            cout << "Permutations generated!"<< endl;
//            if (perms.size() > totalPermutations) {
//                perms.erase(perms.begin() + totalPermutations, perms.end());
//            }
            // no random sampling end
        }

        totalPermutations = perms.size();
        for(int i=0; i<totalPermutations; i++){
            initMappings.push_back(getNextMapping());
//            initMappings[i].setParentMappingId("*");
//            initMappings[i].setMappingId("0." + to_string(i));
        }
    }

    return initMappings;
}

pair<int, int> QuMappingInitializer::getSmartCouple(int first, int second) {
    int physical1 = -1, physical2 = -1;
    physical1 = findNearest(startingNode.getPhysicalQubit());
    physical2 = findNearest(physical1);
    return make_pair(physical1, physical2);
}

void QuMappingInitializer::restrict(int first, int second) {
    pair<int, int> couple;
    if (!isAllocated(first)){
        if (!isAllocated(second)){  // both logical qubits unallocated
            if (INIT_MAPPING_START_NODE_RANK_WISE)
                couple = getSmartCouple(first, second);  // get 2 adjacent pairs maintaining a connected component
            else
                couple = getCouple(first, second);  // get 2 unallocated adjacent pairs

            restrictedMapping.setValueAt(couple.first, first);
            restrictedMapping.setValueAt(couple.second, second);
            allocated[first] = true;
            allocated[second] = true;
        }
        else { // 1st logical qubit free, 2nd allocated
            int physicalQuBit1 = findNearest(second);
            restrictedMapping.setValueAt(physicalQuBit1, first);
            allocated[first] = true;
            removeAdjacents(physicalQuBit1);
        }
    }
    else {
        if (!isAllocated(second)){
            int physicalQuBit2 = findNearest(first);
            restrictedMapping.setValueAt(physicalQuBit2, second);
            allocated[second] = true;
            removeAdjacents(physicalQuBit2);
        }
    }
}

bool QuMappingInitializer::isAllocated(int logicalQuBit){
    return allocated[logicalQuBit];
}

QuMapping QuMappingInitializer::getNextMapping() {
    QuMapping nextMapping(restrictedMapping);

    for(int i=0; i<perms[count].size(); i++){
        int val = perms[count][i];
        nextMapping.setValueAtNextFree(val);
    }
    nextMapping.setUnallocatedQuBits();
    count++;
    restrictedMapping.noMappingInit();
    return nextMapping;
}


void QuMappingInitializer::initGenerator() {
    count = 0;
    int n = architecture.getN();
    for (int i = 0; i < n; ++i) {
        allocated.push_back(false);
    }
    // initialize qubit perm input vector
    auto logicalQubits = circuit.getQubits();
    for (int i = 0; i < logicalQubits.size(); ++i) { // todo: l changed to n ????
        permInput.push_back(i);
    }

    restrictedMapping.setN(n);
    restrictedMapping.noMappingInit();

    makeCouplingMapAdjacencyList();
}

void QuMappingInitializer::removeAdjacents(int physicalQuBit){
    auto it=couples.begin();
    while(it!=couples.end()){
        if(it->first == physicalQuBit || it->second == physicalQuBit)
            it = couples.erase(it);
        else
            ++it;
    }
}

pair<int, int> QuMappingInitializer::getCouple(int first, int second) {
    int physical1 = -1, physical2 = -1;
    if (!couples.empty()){
        physical1 = couples[0].first;
        physical2 = couples[0].second;
        removeAdjacents(physical1);
        removeAdjacents(physical2);
    }
    return make_pair(physical1, physical2);
}

int QuMappingInitializer::findNearest(int physicalQuBit1) {
    int physicalQuBit2 = -1;

    int bestNeighbor = getNeighborFromCommonFreqLists(physicalQuBit1);

    if (bestNeighbor != -1) {
        if (restrictedMapping.getValueAt(bestNeighbor) == -1) { // todo test more corner cases : >3 neighbors
            physicalQuBit2 = bestNeighbor;
        }
        else {
            bestNeighbor = -1;
        }
    }
    if (bestNeighbor == -1) {
        queue<int> todoNodes;
        bool found = false;
        while (!found) {
            for (int i = 0; i < couplingMapAdjList[physicalQuBit1].size(); ++i) {
                int neighbor = couplingMapAdjList[physicalQuBit1][i];
                todoNodes.push(neighbor);
                if (restrictedMapping.getValueAt(neighbor) == -1) { // todo test more corner cases : >3 neighbors
                    physicalQuBit2 = neighbor;
                    found = true;
                }
            }

            if (!found) {
                physicalQuBit1 = todoNodes.front();
                todoNodes.pop();
            }
        }
    }
    return physicalQuBit2;
}

int QuMappingInitializer::findNearest(QuMapping& restrictedMapping, int physicalQuBit1) {
    int physicalQuBit2 = -1;

    int bestNeighbor = getNeighborFromCommonFreqLists(physicalQuBit1);

    if (bestNeighbor != -1) {
        if (restrictedMapping.getValueAt(bestNeighbor) == -1) { // todo test more corner cases : >3 neighbors
            physicalQuBit2 = bestNeighbor;
        }
        else {
            bestNeighbor = -1;
        }
    }
    if (bestNeighbor == -1) {
        queue<int> todoNodes;
        bool found = false;
        while (!found) {
            for (int i = 0; i < couplingMapAdjList[physicalQuBit1].size(); ++i) {
                int neighbor = couplingMapAdjList[physicalQuBit1][i];
                todoNodes.push(neighbor);
                if (restrictedMapping.getValueAt(neighbor) == -1) { // todo test more corner cases : >3 neighbors
                    physicalQuBit2 = neighbor;
                    found = true;
                }
            }

            if (!found) {
                physicalQuBit1 = todoNodes.front();
                todoNodes.pop();
            }
        }
    }
    return physicalQuBit2;
}

QuMappingInitializer::QuMappingInitializer(QuCircuit &circuit, QuArchitecture& architecture): circuit(circuit), architecture(architecture), count(0), restrictedMapping(architecture.getN()){
    initGenerator();
}

int QuMappingInitializer::getNeighborFromCommonFreqLists(int physicalQuBit) {
    int bestNeighbor = -1;
    vector<pair<int, int>> commonSrcListPhysical = architecture.getCommonSrcFreqPriorityList();
    vector<pair<int, int>> commonTargetListPhysical = architecture.getCommonTargetFreqPriorityList();

    for (int i = 0; i < couplingMapAdjList[physicalQuBit].size(); ++i) {
        for (auto& pair: commonSrcListPhysical) {
            int neighbor = couplingMapAdjList[physicalQuBit][i];
            if (pair.first == neighbor){
                bestNeighbor = neighbor;
            }
        }
        if (bestNeighbor == -1){
            for (auto& pair: commonTargetListPhysical) {
                int neighbor = couplingMapAdjList[physicalQuBit][i];
                if (pair.first == neighbor){
                    bestNeighbor = neighbor;
                }
            }
        }
    }
    return bestNeighbor;
}

void QuMappingInitializer::buildRankGraph(vector<pair<int, int>> quBitPairs) {
    for(auto& pair: quBitPairs) {
        rankGraph.caterNode(pair.first, true);
        rankGraph.caterNode(pair.second, false);
    }
    rankGraph.sortByRank();
}

void QuMappingInitializer::initInitializerMappingCounter() {
    count = 0;
    int totalPermutations = TOTAL_PERM;

    if (permInput.size() > PERM_N) // 10! perms
        permInput.erase(permInput.begin() + PERM_N, permInput.end());
    Util::permute(permInput, 0, permInput.size() - 1, perms);
//    cout << perms.size() << " permutations generated!"<< endl;

}


vector<QuMapping> QuMappingInitializer::getNextPermutationMapping() {
    vector<QuMapping> initMappings;
    restrictedMapping.noMappingInit();
    QuMapping nextMapping(restrictedMapping);

    for(int i=0; i<perms[count].size(); i++){
        int val = perms[count][i];
        nextMapping.setValueAtNextFree(val);
    }
    nextMapping.setUnallocatedQuBits();
    count++;
//    nextMapping.setParentMappingId("*");
//    nextMapping.setMappingId("0.0");
    initMappings.push_back(nextMapping);

    return initMappings;
}

vector<QuMapping> QuMappingInitializer::generateAllPermutationInitialMappings() {
    generatePermutations();
    generateMappingsFromPermutations();
    return initialMappings;
}

//// all mappings that give 0 cost for 1st k instructions
//vector<QuMapping> QuMappingInitializer::generateAllZeroCostInitialMappings(int k) {
//    initialMappings.clear();
//    makeCouples();
//    vector<shared_ptr<QuGate>> instructions = circuit.getKBinaryInstructions(k);
//    /////////////////
//    architecture.makeSourceFrequencyPriorityList();
//    architecture.makeTargetFrequencyPriorityList();
//    architecture.makeCommonFrequencyPriorityLists();
////todo todo shashasha
//    auto instruction0 = instructions[0];
//    for (auto pair: couples) {
//        restrictedMapping.init(Constants::INIT_MAPPING_NO_MAPPING);
//        restrict(pair, instruction0); // assigns logical qubits of instruction to 1 physical couple
//        vector<QuMapping> temp = generatePermutationsAfterRestrictions();
//        initialMappings.insert(initialMappings.end(), temp.begin(), temp.end());
//    }
////
////    // restrict more instructions
////    for (int i=1; i< instructions.size();i++) {
////
////
////    }
//
//    return initialMappings;
//}

// all mappings that give 0 cost for 1st k instructions
vector<QuMapping> QuMappingInitializer::generateAllZeroCostInitialMappings(int k) {
    initialMappings.clear();
    vector<QuMapping> restrictedMappings;
//    makeCouples();
    vector<shared_ptr<QuGate>> instructions = circuit.getKBinaryInstructions(k);
    /////////////////
    architecture.makeSourceFrequencyPriorityList();
    architecture.makeTargetFrequencyPriorityList();
    architecture.makeCommonFrequencyPriorityLists();
//todo todo shashasha
    restrictedMapping.init(Constants::INIT_MAPPING_NO_MAPPING);
    restrictedMappings.push_back(restrictedMapping);

    vector<QuMapping> restrictedMappingsForAll;
    for (auto nextInstruction: instructions) {
        restrictedMappingsForAll.clear();
        for (auto restrictedMapping: restrictedMappings) {
            vector<QuMapping> restrictedMappingsFor1 = findRestrictedMappingsFor1Mapping(restrictedMapping, nextInstruction);
            cout << "restrictedMappingsFor1.size(): " << restrictedMappingsFor1.size() << endl;
            restrictedMappingsForAll.insert(restrictedMappingsForAll.end(), restrictedMappingsFor1.begin(), restrictedMappingsFor1.end());
        }
        restrictedMappings = restrictedMappingsForAll;
    }


    for (auto& mapping: restrictedMappingsForAll) {
        vector<QuMapping> temp = generatePermutationsAfterRestrictions(mapping);
        initialMappings.insert(initialMappings.end(), temp.begin(), temp.end());
    }
    for (int i = initialMappings.size()/2; i <initialMappings.size()/2 + 20 ; ++i) {
        initialMappings[i].printShort();
    }
    cout << "initialMappings.size(): " << initialMappings.size() << endl;
    return initialMappings;
}

void QuMappingInitializer::restrict(pair<int, int> couple, shared_ptr<QuGate> instruction) {
    restrictedMapping[couple.first] = instruction->getArgAtIndex(0);
    restrictedMapping[couple.second] = instruction->getArgAtIndex(1);
//    int logicalQubit1 = instruction->getArgAtIndex(0);
//    int logicalQubit2 = instruction->getArgAtIndex(1);


//    restrictedMapping.setUnallocatedQuBits();
}
void QuMappingInitializer::restrict(QuMapping& newRestrictedMapping, pair<int, int> couple, shared_ptr<QuGate> instruction) {
    int logicalQubit1 = instruction->getArgAtIndex(0);
    int logicalQubit2 = instruction->getArgAtIndex(1);
    int physicalQubit1 = couple.first;
    int physicalQubit2 = couple.second;
    if(isOverlapping(newRestrictedMapping, physicalQubit1, physicalQubit2, logicalQubit1, logicalQubit2)) {

    }
    else {
        newRestrictedMapping[physicalQubit1] = logicalQubit1;
        newRestrictedMapping[physicalQubit2] = logicalQubit2;
    }

//    restrictedMapping.setUnallocatedQuBits();
}

vector<QuMapping> QuMappingInitializer::generatePermutationsAfterRestrictions(QuMapping& restrictedMapping) {
    vector<QuMapping> restrictedMappings;
//    vector<int> permInput;
    vector<vector<int>> perms;
    count = 0;

    int n = circuit.getN();
    permInput = circuit.getQubits();
//    int n = architecture.getN();
//    for (int i = 0; i < PERM_N + 2; ++i) {
//        permInput.push_back(i);
//    }

    for (int j = 0; j < architecture.getN(); ++j) {
        if (restrictedMapping[j] != -1)
            permInput.erase(remove(permInput.begin(), permInput.end(), restrictedMapping[j]), permInput.end());
    }

    Util::permute(permInput, 0, permInput.size() - 1, perms);

    for (int k = 0; k < perms.size(); ++k) {
        QuMapping nextMapping(restrictedMapping);
        for(int i=0; i<perms[k].size(); i++){
            int val = perms[k][i];
            nextMapping.setValueAtNextFree(val);
        }
//        nextMapping.setParentMappingId("*");
//        nextMapping.setMappingId("0." + to_string(count));
//        cout << nextMapping.toString() << endl;
        nextMapping.setUnallocatedQuBits(circuit.getQubits());
        restrictedMappings.push_back(nextMapping);
        count++;
    }

    return restrictedMappings;
}

vector<QuMapping> QuMappingInitializer::generateInitialMappings() {
        vector<pair<int, int>> restrictionList;
        vector<pair<int, int>> restrictionListSources;

        // set k unique instructions to restrict // todo SHA: no restrictions
        if (INIT_MAPPING_RESTRICT_MODE) {
            pair<vector<pair<int, int>>, vector<pair<int, int>>> restrictionData = makeRestrictionPairList(K);
            restrictionList = restrictionData.first;
            restrictionListSources = restrictionData.second;
        }
        initialMappings = generateSmartMappings(restrictionListSources, restrictionList);
    return initialMappings;
}

vector<shared_ptr<QuGate>> QuMappingInitializer::getKRestrictInstructions(int k){
    // todo get nonunary from circuit
    vector<shared_ptr<QuGate>> instructions;
//    if (k > nonUnaryInstructions.size())
//        k = nonUnaryInstructions.size();
//    if (k > 0) {
//        if (DAG_SCHEME) {
//            vector<shared_ptr<QuGate> > temp;
//            QuCircuitLayerManager* layerManager = QuCircuitLayerManager::getInstance(nonUnaryInstructions, circuit.getN());
//            vector<int> instructionIds = layerManager->getFirstKInstructionIds(k);
//            for (auto id: instructionIds) {
//                for (shared_ptr<QuGate> inst: nonUnaryInstructions) {
//                    if (inst->getGateId() == id) {
//                        instructions.push_back(inst);
//                    }
//                }
//            }
//        } else {
//            int i = 1;
//            instructions.push_back(nonUnaryInstructions[0]);
//            while (!instructions.empty() && i<nonUnaryInstructions.size() && instructions.size() < k) {
//                if (isNewInsturction(nonUnaryInstructions[i], instructions))
//                    instructions.push_back(nonUnaryInstructions[i]);
//                i++;
//            }
//        }
//    }
    return instructions;
}

pair<vector<pair<int, int>>, vector<pair<int, int>>> QuMappingInitializer::makeRestrictionPairList(int k){ // top k unique instructions
    vector<pair<int, int>> restrictedPairs;
    vector<pair<int, int>> restrictedPairSources;
    int j = 0;
    bool dup = false;

    vector<shared_ptr<QuGate>> instructions = getKRestrictInstructions(k);

    int x = k; // k is max restrictions
//    for (int i = 0; i < k && j<nonUnaryInstructions.size(); ++i) {
//    for (int i = 0; i < k; ++i) {
    for (int i = 0; i < k && i < instructions.size(); ++i) {
        dup = false;
        shared_ptr<QuGate> gate = instructions[i];
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

vector<QuMapping> QuMappingInitializer::findRestrictedMappingsFor1Mapping(QuMapping& restrictedMapping, shared_ptr<QuGate> nextInstruction) {
    vector<QuMapping> mappings;
    vector<int> allocatedPQs;
    vector<int> prospectivePQs;

    allocatedPQs = restrictedMapping.findAllocatedPhysicalQubits();
    if(isOverlapping(restrictedMapping, nextInstruction)) {
        makeCouplesFromOverlappedPhysicalQubits(allocatedPQs, restrictedMapping,
                                                 nextInstruction); // includes overlapping couples as well

    }
    else {
        prospectivePQs = findNeighboursOfAllocatedPhysicalQubits(allocatedPQs);
        makeCouplesFromProspectivePhysicalQubits(prospectivePQs, restrictedMapping,
                                                 nextInstruction); // includes overlapping couples as well
        mappings = restrictAllCouples1By1For1Instruction(restrictedMapping, nextInstruction);
    }
    return mappings;
}

vector<int> QuMappingInitializer::findNeighboursOfAllocatedPhysicalQubits(vector<int> allocatedPQs) {
    vector<int> neighbours;
    for(auto physicalQuBit: allocatedPQs) {
        neighbours.insert(neighbours.end(), couplingMapAdjList[physicalQuBit].begin(), couplingMapAdjList[physicalQuBit].end());
    }
    Util::removeDuplicates(neighbours);
    if(neighbours.empty()) {
        neighbours = architecture.getPhysicalQubitsList();
    }
    return neighbours;
}

// finds the pairs of edges (ignoring hadamard so 2x) from the prospective physical qubits (neighbours of allocated)
// overlapping case
void QuMappingInitializer::makeCouplesFromOverlappedPhysicalQubits(vector<int> allocatedPQs, QuMapping& inputMapping, shared_ptr<QuGate> nextInstruction) {
    couples.clear();
    int logical1 = (*nextInstruction.get())[0];
    int logical2 = (*nextInstruction.get())[1];

    vector<int> prospectivePQs;
    int logical = inputMapping.isLogicalAllocated(logical1) ? logical1 : -1;
    logical = ((logical != -1) && (inputMapping.isLogicalAllocated(logical2))) ? logical2 : -1;
    if(logical != -1) {
        prospectivePQs.insert(prospectivePQs.end(), couplingMapAdjList[inputMapping.getPhysicalBit(logical)].begin(), couplingMapAdjList[inputMapping.getPhysicalBit(logical)].end());
        for (int i=0; i<prospectivePQs.size(); i++) {
            if (inputMapping.isPhysicalAllocated(prospectivePQs[i]))
                prospectivePQs.erase(remove(prospectivePQs.begin(), prospectivePQs.end(), prospectivePQs[i]),
                                     prospectivePQs.end());
        }
        makeCouplesFromProspectivePhysicalQubits(prospectivePQs, restrictedMapping, nextInstruction);
    }



}

// finds the pairs of edges (ignoring hadamard so 2x) from the prospective physical qubits (neighbours of allocated)
// non-overlapping case
void QuMappingInitializer::makeCouplesFromProspectivePhysicalQubits(vector<int> prospectivePQs, QuMapping& inputMapping, shared_ptr<QuGate> nextInstruction) {
    couples.clear();
    int logical1 = (*nextInstruction.get())[0];
    int logical2 = (*nextInstruction.get())[1];
    //todo shashasha - incorporate overlapping here
    for(auto i: prospectivePQs) {
        for(auto j: couplingMapAdjList[i]) {
            if(!inputMapping.isPhysicalAllocated(j) && !inputMapping.isPhysicalAllocated(i)) {
                if (architecture.getCouplingMap()[i][j] > 0) {
                    couples.push_back(make_pair(i, j));
                    if (IGNORE_DIRECTION_IN_ZERO_COST_INIT_MAPPING_RESTRICTION)
                        couples.push_back(make_pair(j, i));
                }
            }
        }
    }
    cout << endl;
}

vector<QuMapping> QuMappingInitializer::restrictAllCouples1By1For1Instruction(QuMapping& restrictedMapping, shared_ptr<QuGate> nextInstruction) {
    vector<QuMapping> mappings;
    mappings.clear();
    for (auto pair: couples) {
        QuMapping newRestrictedMapping(restrictedMapping);
//        restrictedMapping.init(Constants::INIT_MAPPING_NO_MAPPING);
        Util::printPairs(couples);
        restrict(newRestrictedMapping, pair, nextInstruction); // assigns logical qubits of instruction to 1 physical couple
//        vector<QuMapping> temp = generatePermutationsAfterRestrictions(newRestrictedMapping);
//        mappings.insert(mdappings.end(), temp.begin(), temp.end());
        mappings.push_back(newRestrictedMapping);
    }
    return mappings;
}

bool QuMappingInitializer::isOverlapping(QuMapping &mapping, shared_ptr<QuGate> nextInstruction) {
    int logical1 = (*nextInstruction.get())[0];
    int logical2 = (*nextInstruction.get())[1];

    return mapping.isLogicalAllocated(logical1) || mapping.isLogicalAllocated(logical2);
}

//bool QuMappingInitializer::isOverlapping(QuMapping &mapping, int physicalQubit1, int physicalQubit2, int logicalQubit1,
//                                         int logicalQubit2) {
//
//
//    return false;
//}
