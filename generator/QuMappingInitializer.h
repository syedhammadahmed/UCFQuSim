//
// Created by SHA on 7/24/2020.
//

#ifndef UCFQUSIM_QUMAPPINGINITIALIZER_H
#define UCFQUSIM_QUMAPPINGINITIALIZER_H


#include "core/QuMapping.h"
#include "PriorityGraph.h"

class QuMappingInitializer {
private:
    QuCircuit &circuit;
    QuArchitecture& architecture;

    int count; // permutation count
    vector<int> permInput; // permutation input vector consisting of qubits to permute // restricted qubits are excluded from this
    vector<vector<int>> perms;

    vector<bool> allocated;
    vector<pair<int, int>> couples;
    vector<vector<int>> couplingMapAdjList;
    QuMapping restrictedMapping;

    PriorityGraph rankGraph;
    PriorityNode startingNode;

    vector<QuMapping> initialMappings;
public:
    QuMappingInitializer(QuCircuit &circuit, QuArchitecture &architecture);

    void initGenerator();
    void generateMappingsFromPermutations();
    void generatePermutations();
    QuMapping getNextMapping();

    vector<QuMapping> generateSmartMappings(vector<pair<int, int>> restrictionListSources, vector<pair<int, int>> restrictionPairs);

    void restrict(int first, int second);
    bool isAllocated(int logicalQuBit);
    pair<int, int> getCouple(int first, int second);
    void makeCouples();
    void removeAdjacents(int physicalQuBit);
    int findNearest(int logicalQuBit);

    pair<int, int> getSmartCouple(int first, int second);
    int getNeighborFromCommonFreqLists(int physicalQuBit);

    void buildPhysicalQuBitPriorityLists(QuArchitecture &architecture);

    void buildRankGraph(vector<pair<int, int>> quBitPairs);

    PriorityNode getTopRankNode();

    PriorityNode allocateTopRankNode();

    vector<pair<int, int>> getFreeNeighbors();
//    vector<QuMapping> generateHardcodedMappings(vector<int> physicalToLogical, QuArchitecture &quArchitecture);
    vector<QuMapping> getNextPermutationMapping();

    void initInitializerMappingCounter();
    vector<QuMapping> generateInitialMappings();
    pair<vector<pair<int, int>>, vector<pair<int, int>>> makeRestrictionPairList(int k);
    vector<shared_ptr<QuGate>> getKRestrictInstructions(int k);

    vector<QuMapping> generateAllPermutationInitialMappings();
    vector<QuMapping> generateAllZeroCostInitialMappings(int k);

    void restrict(pair<int, int> couple, shared_ptr<QuGate> instruction);

    vector<QuMapping> generatePermutationsAfterRestrictions(QuMapping& restrictedMapping);

private:
    vector<QuMapping> findRestrictedMappingsFor1Mapping(QuMapping& inputMapping, shared_ptr<QuGate> nextInstruction);

    vector<int> findNeighboursOfAllocatedPhysicalQubits(vector<int> allocatedPQs);

    void makeCouplesFromProspectivePhysicalQubits(vector<int> prospectivePQs, QuMapping& inputMapping, shared_ptr<QuGate> nextInstruction);
    void makeCouplesFromOverlappedPhysicalQubits(vector<int> allocatedPQs, QuMapping& inputMapping, shared_ptr<QuGate> nextInstruction);
    void makeOverlappedCouplesFromProspectivePhysicalQubits(vector<int> prospectivePQs, int physical);

    vector<QuMapping> restrictAllCouples1By1For1Instruction(QuMapping& restrictedMapping, shared_ptr<QuGate> nextInstruction);

    void makeCouplingMapAdjacencyList();

    void restrict(QuMapping &newRestrictedMapping, pair<int, int> couple, shared_ptr<QuGate> instruction);

    bool isOverlapping(QuMapping &mapping, shared_ptr<QuGate> instruction);
//    bool isOverlapping(QuMapping &mapping, int physicalQubit1, int physicalQubit2, int logicalQubit1, int logicalQubit2);

    int findNearest(QuMapping &restrictedMapping, int physicalQuBit1);

    vector<QuMapping>
    restrictAllOverlappedCouples1By1For1Instruction(QuMapping &restrictedMapping, shared_ptr<QuGate> nextInstruction);

    void restrictOverlapped(QuMapping &newRestrictedMapping, pair<int, int> couple, shared_ptr<QuGate> instruction);
};


#endif //UCFQUSIM_QUMAPPINGINITIALIZER_H
