//
// Created by SHA on 11/19/19.
//

#ifndef UCFQUSIM_QUSMARTSWAPPER_H
#define UCFQUSIM_QUSMARTSWAPPER_H


#include <algorithm/AllShortestPathsFinder.h>
#include <algorithm/AllPairShortestPathFinder.h>
#include <unordered_map>
#include "QuSwapStrategy.h"
#include "QuMappingInitializerZulehner.h"
#include "Config.h"
#include "QuMappingInitializer.h"

class AllPairShortestPathsFinder;

class QuSmartSwapper: public QuSwapStrategy {
private:
    vector<vector<QuMapping>> instructionWiseMappings;  // instructionWiseMappings[0] = mappings for instruction 0 (1st instruction)
    int perInstructionMappingCounter;
    vector<shared_ptr<QuGate>> nonUnaryInstructions;
    AllShortestPathsFinder* allSPF;
    AllPairShortestPathFinder* allPairShortestPathFinder;
    unsigned int hadamards;
    vector<vector<int>> mappingWiseShortestPathCosts;
    vector<vector<vector<int>>> mappingWiseShortestPaths;

    vector<int> selectedNonUnaryInstructionIds;
    vector<int> currentInstructionIds; // to be used in init mappings that's why attribute

    unordered_map<int, shared_ptr<QuGate>> nonUnaryInstructionsMap;
    vector<QuMapping> initialMappings;


public:
    QuSmartSwapper(QuCircuit &circuit, QuArchitecture& architecture, vector<QuMapping>& initialMappings);

//    int findTotalSwaps() override;
    pair<int, QuMapping> findTotalSwaps() override;
    void findShortestPathsFor1InputMapping() override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

    vector<QuMapping> getAllMappingsForCurrentInstruction();
    pair<vector<QuMapping>, int> findAllMinCostMappingsFromPermutationsFor1Path(QuMapping& mapping, vector<int> sequence);
    void insertSwapGates(int source, int destination);
    vector<shared_ptr<QuGate>> removeUnaryInstructions();
    unsigned int constraintNotSatisfied(int src, int dest, int **couplingMap);
    int insertRemovedUnaryInstructions(vector<shared_ptr<QuGate>>& finalProgram, int nextNonUnaryIndex);
    void insertEndingUnaryInstructions(vector<shared_ptr<QuGate> > &finalProgram);
    void hadamardCheck(vector<shared_ptr<QuGate>>& finalProgram, QuArchitecture& quArchitecture, QuMapping& currentMapping, int index);
    QuMapping generateOptimalInstructions();
    void mappingSanityCheck();
    int calculateHadamardCost(vector<int> shortestPath, int **couplingMap);
    int findShortestPathsMinimumCost();
    pair<int, vector<struct QuMapping>> findMinCostMappingsForNextInstruction(vector<QuMapping> &inputMappings);
    int optimize(vector<shared_ptr<QuGate>>& finalProgram);
    vector<int> getCurrentInstructionIds();
    bool isNewInsturction(shared_ptr<QuGate> currentInstruction, vector<shared_ptr<QuGate> > &instructions);
    int findTotalCostDefault();
    int findTotalCostDAG();
    void updateMappingIdsForDitto();

    bool currentInstructionSameAsPrevious(shared_ptr<QuGate> previous, shared_ptr<QuGate> current);
    void doExtraHadamardFiltering(shared_ptr<QuGate> currentInstruction, QuArchitecture& quArchitecture);

    void init();

    void findShortestPathsForAllInputMappings(vector<QuMapping> inputMappings);

    QuMapping generateOptimalInstructionsDAG();

    QuMapping generateOptimalInstructionsDefault();

    int insertRemovedUnaryInstructionsDefault(vector<shared_ptr<QuGate> > &finalProgram, int nextNonUnaryIndex);

    int insertRemovedUnaryInstructionsDAG(vector<shared_ptr<QuGate> > &finalProgram, int nextNonUnaryIndex);

    int findTotalCostDefaultAllPermutations();

    vector<QuMapping> pruneMappings(bool random);
};

#endif //UCFQUSIM_QUSMARTSWAPPER_H
