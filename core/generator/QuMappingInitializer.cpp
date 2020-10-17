//
// Created by YUSHAH on 7/24/2020.
//

#include <util/Util.h>
#include <queue>
#include <algorithm>
#include "QuMappingInitializer.h"
#include "PriorityGraph.h"

using namespace std;

//int QuMappingInitializer::TOTAL_PERM = 1000000000; // # of qubits
int QuMappingInitializer::TOTAL_PERM = 500; // # of qubits
//int QuMappingInitializer::n = 0; // # of qubits
//int QuMappingInitializer::count = 0; // permutation count
//QuMapping QuMappingInitializer::defaultMapping;
//vector<string> QuMappingInitializer::perms;



void QuMappingInitializer::generateMappings() {
    vector<int> permInput;
    for (int i = 0; i < n; ++i) {
        permInput.push_back(i);
    }
    Util::permute(permInput, 0, n-1, perms);
}
//void QuMappingInitializer::generateMappings() {
//    string str = "0123456789";
//    int n = str.size();
//    Util::permute(str, 0, n-1, QuMappingInitializer::perms);
//}

void QuMappingInitializer::makeSmartCouples(QuArchitecture& quArchitecture){
    couplingMapAdjList.resize(quArchitecture.getN());

    for(int i = 0; i < quArchitecture.getN(); i++) {
        for (int j = 0; j < quArchitecture.getN(); j++){
            if ((i != j) && (quArchitecture.getCouplingMap()[i][j] > 0)){
                 couples.push_back(make_pair(i, j));
                couplingMapAdjList[i].push_back(j);
                couplingMapAdjList[j].push_back(i);
            }
        }
    }
// make couples according to src freq priority
//    vector<pair<int, int>> srcPriorityListPhysical = quArchitecture.getSrcFreqPriorityList();
//    vector<pair<int, int>> targetPriorityListPhysical = quArchitecture.getTargetFreqPriorityList();
//
//    for (const auto& pair: srcPriorityListPhysical) { // make couples according to src freq priority
//        vector<int> neighbors = couplingMapAdjList[pair.first];
//        for (auto& item: neighbors) {
//            couples.push_back(make_pair(pair.first, item));
//        }
//    }
// make couples according to target freq priority todo: bad bad bad remove it
//    vector<pair<int, int>> srcPriorityListPhysical = quArchitecture.getSrcFreqPriorityList();
//    vector<pair<int, int>> targetPriorityListPhysical = quArchitecture.getTargetFreqPriorityList();
//
//    for (const auto& pair: targetPriorityListPhysical) { // make couples according to src freq priority
//        vector<int> neighbors = couplingMapAdjList[pair.second];
//        for (auto& item: neighbors) {
//            couples.push_back(make_pair(item, pair.second));
//        }
//    }
}

//void QuMappingInitializer::makeCouples(QuArchitecture& quArchitecture){
////    vector<pair<int , int>> counts;
//    for(int i = 0; i < quArchitecture.getN(); i++) {
//        vector<int> list;
//        couplingMapAdjList.push_back(list);
////        counts.push_back(make_pair(i,0));
//    }
//    for(int i = 0; i < quArchitecture.getN(); i++) {
//        for (int j = 0; j < quArchitecture.getN(); j++){
//            if ((i != j) && (quArchitecture.getCouplingMap()[i][j] > 0)){
////                counts[i].second++;
//                couples.push_back(make_pair(i, j));
//                couplingMapAdjList[i].push_back(j);
//                couplingMapAdjList[j].push_back(i);
//            }
//        }
//    }
//}
//
struct MappingEqualityComparator {
    bool pred(QuMapping &a, QuMapping &b) {
        for (int i = 0; i < a.getN(); ++i) {
            if (a.getValueAt(i) != b.getValueAt(i))
                return false;
        }
        return true;
    }
};

//vector<QuMapping> QuMappingInitializer::generateSmartMappings(vector<pair<int, int>> restrictionListSources, vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture) {
//    vector<QuMapping> initMappings;
//
//    quArchitecture.makeSourceFrequencyPriorityList();
//    quArchitecture.makeTargetFrequencyPriorityList();
//    quArchitecture.makeCommonFrequencyPriorityLists();
//
//    makeCouples(quArchitecture);  // makes an adj list of coupling map for restriction mappings
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
//    commonSrcListPhysical = quArchitecture.getCommonSrcFreqPriorityList();
//    commonTargetListPhysical = quArchitecture.getCommonTargetFreqPriorityList();
//    srcListPhysical = quArchitecture.getSrcFreqPriorityList();
//    targetListPhysical = quArchitecture.getTargetFreqPriorityList();
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

    PriorityNode logicalQuBitToAllocate = rankGraph.getHead();  // top (i+1)-th node and deleted on returning

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

vector<QuMapping> QuMappingInitializer::generateSmartMappings(vector<pair<int, int>> restrictionListSources, vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture) {
    vector<QuMapping> initMappings;

    buildPhysicalQuBitPriorityLists(quArchitecture);
    buildRankGraph(restrictionListSources);

    makeSmartCouples(quArchitecture);  // makes an adj list of coupling map for restriction mappings

    startingNode = allocateTopRankNode(); // starting point of init mapping allocation

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
//    restrictedMapping.strongInit();

    for(int i=0; i<restrictionPairs.size(); i++){
        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
        if (!permInput.empty()) {
            permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
            permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
        }
    }

//
    //    for(int i=0; i<restrictionPairs.size(); i++){
//        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].first), permInput.end());
//        permInput.erase(remove(permInput.begin(), permInput.end(), restrictionPairs[i].second), permInput.end());
//
//        restrict(restrictionPairs[i].first, restrictionPairs[i].second);
//    }

    if (permInput.size()>10) // 10! perms
        permInput.erase(permInput.begin()+10, permInput.end());
    Util::permute(permInput, 0, permInput.size()-1, perms);

    if (perms.size() > TOTAL_PERM) { // todo random sampling - DONE
        perms.erase(perms.begin() + TOTAL_PERM - 1, perms.end());
    }
    TOTAL_PERM = perms.size();

    //    perms = Util::getNRandomPermutations(TOTAL_PERM, permInput);  // todo uncomment for sampling

    for(int i=0; i<TOTAL_PERM; i++){
        initMappings.push_back(getNextMapping());
    }
    // todo remove duplicate mappings
//    std::unique(initMappings.begin(), initMappings.end(), MappingEqualityComparator());


//    initMappings.clear(); // todo remove it.. just testing..
//    QuMapping initialMapping;
//    initialMapping.defaultInit();
//    initMappings.insert(initMappings.begin(), initialMapping);

    // remove duplicate initial mappings

    auto it = std::unique (initMappings.begin(), initMappings.end());   // (no changes)
    initMappings.resize( std::distance(initMappings.begin(),it) );

    return initMappings;
}


pair<int, int> QuMappingInitializer::getSmartCouple(int first, int second) {
    int physical1 = -1, physical2 = -1;
    physical1 = findNearest(startingNode.getPhysicalQubit());
    physical2 = findNearest(physical1);
//    if (!couples.empty()){
//        int i;
//        for (i = 0; i < couples.size(); ++i) {
//            if (couples[i].first == startingNode.getPhysicalQubit()){
//                break;
//            }
//            else if (couples[i].second == startingNode.getPhysicalQubit()){
//                couples[i].first =
//                break;
//            }
//        }
//        physical1 = couples[i].first;
//        physical2 = couples[i].second;
//        removeAdjacents(physical1);
//        removeAdjacents(physical2);
//    }
    return make_pair(physical1, physical2);
}

void QuMappingInitializer::smartRestrict(int first, int second) {
    if (!isAllocated(first)){
        if (!isAllocated(second)){
            pair<int, int> couple = getSmartCouple(first, second);  // get 2 adjacent pairs
            restrictedMapping.setValueAt(couple.first, first);
            restrictedMapping.setValueAt(couple.second, second);
            allocated[first] = true;
            allocated[second] = true;
        }
        else {
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

void QuMappingInitializer::restrict(int first, int second) {
    if (!isAllocated(first)){
        if (!isAllocated(second)){
//            pair<int, int> couple = getCouple(first, second);  // get 2 adjacent pairs
            pair<int, int> couple = getSmartCouple(first, second);  // get 2 adjacent pairs

            restrictedMapping.setValueAt(couple.first, first);
            restrictedMapping.setValueAt(couple.second, second);
            allocated[first] = true;
            allocated[second] = true;
        }
        else {
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
//    restrictedMapping.strongInit();
//    restrictedMapping.setValueAt(startingNode.getPhysicalQubit(), startingNode.getLogicalQubit());
    QuMapping nextMapping(restrictedMapping);

    for(int i=0; i<perms[count].size(); i++){
        int val = perms[count][i];
        nextMapping.setValueAtNextFree(val);
    }
    nextMapping.setUnallocatedQuBits();
//    nextMapping.setParentMappingId("*");
//    nextMapping.setMappingId("0." + to_string(count)); //temp[i].setMappingId(to_string(programCounter) + "." + to_string(i));
    count++;
    return nextMapping;
}


void QuMappingInitializer::initGenerator(int n) {
    TOTAL_PERM = 1000;
    count = 0;
    for (int i = 0; i < n; ++i) {
        allocated.push_back(false);
    }
    // initialize qubit perm input vector
    for (int i = 0; i < n; ++i) {
        permInput.push_back(i);
    }

    restrictedMapping.setN(n);
    restrictedMapping.strongInit();
}

const int QuMappingInitializer::getPermCount() {
    return perms.size();
}

void QuMappingInitializer::removeAdjacents(int physicalQuBit){
    vector<pair<int, int>>::iterator it=couples.begin();
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
//    cout << "logical: " << logicalQuBit << ", physical: " << physicalQuBit1 << ", bestNeighbor: " << bestNeighbor << endl;

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

QuMappingInitializer::QuMappingInitializer(int n): n(n) {
    initGenerator(n);
}

QuMappingInitializer::QuMappingInitializer(int n, int l): n(n), l(l), count(0) {
    initGenerator(n);
}



QuMappingInitializer::QuMappingInitializer() = default;

int QuMappingInitializer::getNeighborFromCommonFreqLists(int physicalQuBit) {
    int bestNeighbor = -1;
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

void QuMappingInitializer::buildPhysicalQuBitPriorityLists(QuArchitecture &quArchitecture) {
    quArchitecture.makeSourceFrequencyPriorityList();
    quArchitecture.makeTargetFrequencyPriorityList();
    quArchitecture.makeCommonFrequencyPriorityLists();
    commonSrcListPhysical = quArchitecture.getCommonSrcFreqPriorityList();
    commonTargetListPhysical = quArchitecture.getCommonTargetFreqPriorityList();
    srcListPhysical = quArchitecture.getSrcFreqPriorityList();
    targetListPhysical = quArchitecture.getTargetFreqPriorityList();

}

void QuMappingInitializer::buildRankGraph(vector<pair<int, int>> quBitPairs) {
    for(auto& pair: quBitPairs) {
        rankGraph.caterNode(pair.first, true);
        rankGraph.caterNode(pair.second, false);
    }
    rankGraph.sortByRank();
}

