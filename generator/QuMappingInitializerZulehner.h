//
// Created by SHA on 7/24/2020.
//

#ifndef UCFQUSIM_QuMappingInitializerZulehner_ZULEHNER_H
#define UCFQUSIM_QuMappingInitializerZulehner_ZULEHNER_H


#include <core/QuMapping.h>
#include "PriorityGraph.h"

class QuMappingInitializerZulehner {
private:
    int n; // # of qubits
    int l; // logical qubits
    int count; // permutation count
    QuMapping defaultMapping;
//    vector<string> perms;
    vector<vector<int>> perms;
    vector<bool> allocated;
    vector<pair<int, int>> couples;
    vector<vector<int>> couplingMapAdjList;
    QuMapping restrictedMapping;
    PriorityGraph rankGraph;

    vector<pair<int, int>> srcListPhysical;
    vector<pair<int, int>> targetListPhysical;
    vector<pair<int, int>> commonSrcListPhysical;
    vector<pair<int, int>> commonTargetListPhysical;

    vector<int> permInput; // permutation input vector consisting of qubits to permute // restricted qubits are excluded from this
    PriorityNode startingNode;

    vector<shared_ptr<QuGate>> layer0Instructions;

public:
    void initGenerator();
    void generateMappings();
    QuMapping getNextMapping();
    const vector<string> &getPerms();

//    void initGenerator(int n);

    const int getPermCount();

    vector<QuMapping> generateSmartMappings(vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture);

//    QuMapping getNextMapping(vector<pair<int, int>> restrictionPairs);

    void restrict(int first, int second);

    bool isAllocated(int logicalQuBit);

    pair<int, int> getCouple(int first, int second);

    void makeCouples(QuArchitecture &quArchitecture);

    void removeAdjacents(int physicalQuBit);

    int findNearest(int logicalQuBit);

    static bool pred(QuMapping &a, QuMapping &b);

    bool myfunction(int i, int j);

    void smartRestrict(int first, int second);

    pair<int, int> getSmartCouple(int first, int second);

    int getNeighborFromCommonFreqLists(int physicalQuBit);

    void makeSmartCouples(QuArchitecture &architecture);

    void buildPhysicalQuBitPriorityLists(QuArchitecture &architecture);

    void buildRankGraph(vector<pair<int, int>> quBitPairs);

    PriorityNode getTopRankNode();

    PriorityNode allocateTopRankNode();

    vector<pair<int, int>> getFreeNeighbors();

//    vector<QuMapping> generateHardcodedMappings(vector<int> physicalToLogical, QuArchitecture &quArchitecture);
    void setLayer0Instructions(vector<shared_ptr<QuGate>> vector);
};


#endif //UCFQUSIM_QuMappingInitializerZulehner_H
