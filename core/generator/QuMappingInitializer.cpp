//
// Created by YUSHAH on 7/24/2020.
//

#include <util/Util.h>
#include <queue>
#include "QuMappingInitializer.h"

const int QuMappingInitializer::TOTAL_PERM = 3; // # of qubits
//int QuMappingInitializer::n = 0; // # of qubits
//int QuMappingInitializer::count = 0; // permutation count
//QuMapping QuMappingInitializer::defaultMapping;
//vector<string> QuMappingInitializer::perms;



void QuMappingInitializer::generateMappings() {
    string str = "0123456789";
    int n = str.size();
    Util::permute(str, 0, n-1, QuMappingInitializer::perms);
}

void QuMappingInitializer::makeCouples(QuArchitecture& quArchitecture){
    for(int i = 0; i < quArchitecture.getN(); i++) {
        vector<int> list;
        couplingMapAdjList.push_back(list);
    }

    for(int i = 0; i < quArchitecture.getN(); i++) {
        for (int j = 0; j < quArchitecture.getN(); j++){
            if ((i != j) && (quArchitecture.getCouplingMap()[i][j] > 0)){
                couples.push_back(make_pair(i, j));
                couplingMapAdjList[i].push_back(j);
                couplingMapAdjList[j].push_back(i);
            }
        }
    }
}

vector<QuMapping> QuMappingInitializer::generateSmartMappings(vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture) {
    vector<QuMapping> initMappings;
    makeCouples(quArchitecture);
    QuMapping restrictedMapping(true);

    // remove restricted qubits from perm input string
    string str = "0123456789";
    int n = str.size();
    for(int i=0; i<restrictionPairs.size(); i++){
        int pos1 = str.find(to_string(restrictionPairs[i].first));
        if (pos1 != -1) str.replace(pos1, 1, "");
        pos1 = str.find(to_string(restrictionPairs[i].second));
        if (pos1 != -1) str.replace(pos1, 1, "");

        restrict(restrictedMapping, restrictionPairs[i].first, restrictionPairs[i].second);
    }
    //
    // permute after removing restrictions
    Util::permute(str, 0, n-1, QuMappingInitializer::perms);

    for(int i=0; i<QuMappingInitializer::TOTAL_PERM; i++){
        initMappings.push_back(getNextMapping(restrictionPairs));
    }


    for(QuMapping& m: initMappings){
        m.setParentMappingId("*");
    }
    return initMappings;
}

void QuMappingInitializer::restrict(QuMapping& mapping, int first, int second) {
    if (!isAllocated(first)){
        if (!isAllocated(second)){
            pair<int, int> couple = getCouple(first, second);
            mapping.setValueAt(couple.first, first);
            mapping.setValueAt(couple.second, second);
            allocated[first] = true;
            allocated[second] = true;
        }
        else {
            int physicalQuBit1 = findNearest(mapping, second);
            mapping.setValueAt(physicalQuBit1, first);
            allocated[first] = true;
            removeAdjacents(physicalQuBit1);
        }
    }
    else {
        if (!isAllocated(second)){
            int physicalQuBit2 = findNearest(mapping, first);
            mapping.setValueAt(physicalQuBit2, second);
            allocated[second] = true;
            removeAdjacents(physicalQuBit2);
        }
    }
}

bool QuMappingInitializer::isAllocated(int logicalQuBit){
    return allocated[logicalQuBit];
}

QuMapping QuMappingInitializer::getNextMapping(vector<pair<int, int>> restrictionPairs) {
    QuMapping nextMapping(true);
    for(int i=0; i<restrictionPairs.size(); i++) {
        restrict(nextMapping, restrictionPairs[i].first, restrictionPairs[i].second);


    }

    for(int i=0; i<perms[count].length(); i++){
        int val = perms[count][i] - 48;
        nextMapping.setValueAtNextFree(val);
    }
    count++;
    return nextMapping;
}

void QuMappingInitializer::initGenerator(int n) {
    this->n = n;
    count = 0;
    for (int i = 0; i < n; ++i) {
        allocated.push_back(false);
    }
}

const int QuMappingInitializer::getPermCount() {
    return perms.size();
}

QuMappingInitializer::QuMappingInitializer(int n) {
    initGenerator(n);
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

int QuMappingInitializer::findNearest(QuMapping& mapping, int logicalQuBit) {
    int physicalQuBit1 = mapping.getPhysicalBit(logicalQuBit);
    int physicalQuBit2 = -1;

    queue<int> todoNodes;
    bool found = false;
    while(!found){
        for (int i = 0; i < couplingMapAdjList[physicalQuBit1].size(); ++i) {
            int neighbor = couplingMapAdjList[physicalQuBit1][i];
            todoNodes.push(neighbor);
            if (mapping.getValueAt(neighbor) == -1) {
                physicalQuBit2 = neighbor;
                found = true;
            }
        }
        if (!found) {
            physicalQuBit1 = todoNodes.front();
            todoNodes.pop();
        }
    }

    return physicalQuBit2;
}


QuMappingInitializer::QuMappingInitializer() = default;
