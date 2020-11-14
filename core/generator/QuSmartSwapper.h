//
// Created by SHA on 11/19/19.
//

#ifndef UCFQUSIM_QUSMARTSWAPPER_H
#define UCFQUSIM_QUSMARTSWAPPER_H


#include <AllShortestPathsFinder.h>
#include <AllPairShortestPathFinder.h>
#include <unordered_map>
#include "QuSwapStrategy.h"
#include "QuMappingInitializer.h"
#include "Config.h"

class AllPairShortestPathsFinder;

class QuSmartSwapper: public QuSwapStrategy {
private:
//    static int MAPPING_THRESHOLD;
    QuMapping initialMapping;
    vector<vector<QuMapping>> instructionWiseMappings;  // instructionWiseMappings[0] = mappings for instruction 0 (1st instruction)
    int perInstructionMappingCounter;
    vector<QuGate*> nonUnaryInstructions;
    AllShortestPathsFinder* allSPF;
    AllPairShortestPathFinder* allPairShortestPathFinder;
    map<string, vector<int>> preCalShortestPaths;
    QuMappingInitializer mappingInitializer;
    vector<QuMapping> initialMappings;
    unsigned int hadamards;
    unsigned int totalSwaps;
    unsigned int totalHadamards;
    vector<vector<int>> mappingWiseShortestPathCosts;
    vector<vector<vector<int>>> mappingWiseShortestPaths;

    vector<int> selectedNonUnaryInstructionIds;
    vector<int> currentInstructionIds; // to be used in init mappings that's why attribute

    unordered_map<int, QuGate*> nonUnaryInstructionsMap;

public:
    QuSmartSwapper(QuCircuit &circuit, QuArchitecture& architecture);

    int findTotalSwaps() override;
    void findShortestPathsFor1InputMapping() override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

    vector<QuMapping> getAllMappingsForCurrentInstruction();
    pair<vector<QuMapping>, int> findAllMinCostMappingsFromPermutationsFor1Path(QuMapping& mapping, vector<int> sequence);
    void insertSwapGates(int source, int destination);
    vector<QuGate*> removeUnaryInstructions();
    unsigned int constraintNotSatisfied(int src, int dest, int **couplingMap);
    int insertRemovedUnaryInstructions(vector<QuGate*>& finalProgram, int nextNonUnaryIndex);
    void insertEndingUnaryInstructions(vector<QuGate *> &finalProgram);
    void hadamardCheck(vector<QuGate*>& finalProgram, QuArchitecture& quArchitecture, QuMapping& currentMapping, int index);
    QuMapping generateOptimalInstructions();
    vector<QuMapping> generateInitialMappings();
    pair<vector<pair<int, int>>, vector<pair<int, int>>> makeRestrictionPairList(int k);
    unsigned int getHadamards() const;
    void mappingSanityCheck();
    int calculateHadamardCost(vector<int> shortestPath, int **couplingMap);
    int findShortestPathsMinimumCost();
    int findMinCostMappingsForNextInstruction(vector<QuMapping> &inputMappings);
    void optimize(vector<QuGate*>& finalProgram);
    int performCNOTCancellations(vector<QuGate *> &vector);
    int performUnaryCancellations(vector<QuGate *> &finalProgram);
    vector<QuGate *>::iterator findMergingPartner(vector<QuGate *>::iterator it1, vector<QuGate *>::iterator end);
    vector<int> getCurrentInstructionIds();
    vector<QuGate *> getKRestrictInstructions(int k);
    bool isNewInsturction(QuGate *currentInstruction, vector<QuGate *> &instructions);
    int findTotalCostDefault();
    int findTotalCostDAG();
    void updateMappingIdsForDitto();

    bool currentInstructionSameAsPrevious(QuGate *previous, QuGate *current);
    void doExtraHadamardFiltering(QuGate* currentInstruction, QuArchitecture& quArchitecture);

    void init();

    void findShortestPathsForAllInputMappings(vector<QuMapping> inputMappings);
};

#endif //UCFQUSIM_QUSMARTSWAPPER_H
