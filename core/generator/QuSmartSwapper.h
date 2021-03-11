//
// Created by SHA on 11/19/19.
//

#ifndef UCFQUSIM_QUSMARTSWAPPER_H
#define UCFQUSIM_QUSMARTSWAPPER_H


#include <AllShortestPathsFinder.h>
#include <AllPairShortestPathFinder.h>
#include <unordered_map>
#include "QuSwapStrategy.h"
#include "QuMappingInitializerZulehner.h"
#include "Config.h"

class AllPairShortestPathsFinder;

class QuSmartSwapper: public QuSwapStrategy {
private:
//    static int MAPPING_THRESHOLD;
//    QuMapping initialMapping;
    vector<vector<QuMapping>> instructionWiseMappings;  // instructionWiseMappings[0] = mappings for instruction 0 (1st instruction)
    int perInstructionMappingCounter;
    vector<std::shared_ptr<QuGate>> nonUnaryInstructions;
    AllShortestPathsFinder* allSPF;
    AllPairShortestPathFinder* allPairShortestPathFinder;
    map<string, vector<int>> preCalShortestPaths;
//    QuMappingInitializer mappingInitializer;
    QuMappingInitializerZulehner mappingInitializer;
    vector<QuMapping> initialMappings;
    unsigned int hadamards;
    unsigned int totalSwaps;
    unsigned int totalHadamards;
    vector<vector<int>> mappingWiseShortestPathCosts;
    vector<vector<vector<int>>> mappingWiseShortestPaths;

    vector<int> selectedNonUnaryInstructionIds;
    vector<int> currentInstructionIds; // to be used in init mappings that's why attribute

    unordered_map<int, std::shared_ptr<QuGate>> nonUnaryInstructionsMap;


public:
    QuSmartSwapper(QuCircuit &circuit, QuArchitecture& architecture);

    int findTotalSwaps() override;
    void findShortestPathsFor1InputMapping() override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

    vector<QuMapping> getAllMappingsForCurrentInstruction();
    pair<vector<QuMapping>, int> findAllMinCostMappingsFromPermutationsFor1Path(QuMapping& mapping, vector<int> sequence);
    void insertSwapGates(int source, int destination);
    vector<std::shared_ptr<QuGate>> removeUnaryInstructions();
    unsigned int constraintNotSatisfied(int src, int dest, int **couplingMap);
    int insertRemovedUnaryInstructions(vector<std::shared_ptr<QuGate>>& finalProgram, int nextNonUnaryIndex);
    void insertEndingUnaryInstructions(vector<std::shared_ptr<QuGate> > &finalProgram);
    void hadamardCheck(vector<std::shared_ptr<QuGate>>& finalProgram, QuArchitecture& quArchitecture, QuMapping& currentMapping, int index);
    QuMapping generateOptimalInstructions();
    vector<QuMapping> generateInitialMappings();
    pair<vector<pair<int, int>>, vector<pair<int, int>>> makeRestrictionPairList(int k);
    unsigned int getHadamards() const;
    void mappingSanityCheck();
    int calculateHadamardCost(vector<int> shortestPath, int **couplingMap);
    int findShortestPathsMinimumCost();
    pair<int, vector<struct QuMapping>> findMinCostMappingsForNextInstruction(vector<QuMapping> &inputMappings);
    int optimize(vector<std::shared_ptr<QuGate>>& finalProgram);
    vector<int> getCurrentInstructionIds();
    vector<std::shared_ptr<QuGate> > getKRestrictInstructions(int k);
    bool isNewInsturction(std::shared_ptr<QuGate> currentInstruction, vector<std::shared_ptr<QuGate> > &instructions);
    int findTotalCostDefault();
    int findTotalCostDAG();
    void updateMappingIdsForDitto();

    bool currentInstructionSameAsPrevious(std::shared_ptr<QuGate> previous, std::shared_ptr<QuGate> current);
    void doExtraHadamardFiltering(std::shared_ptr<QuGate> currentInstruction, QuArchitecture& quArchitecture);

    void init();

    void findShortestPathsForAllInputMappings(vector<QuMapping> inputMappings);
    static int performCNOTCancellations(vector<std::shared_ptr<QuGate> > &vector);
    static int performUnaryCancellations(vector<std::shared_ptr<QuGate> > &finalProgram);
    static vector<std::shared_ptr<QuGate> >::iterator findMergingPartner(vector<std::shared_ptr<QuGate> >::iterator it1, vector<std::shared_ptr<QuGate> >::iterator end);

    QuMapping generateOptimalInstructionsDAG();

    QuMapping generateOptimalInstructionsDefault();

    int insertRemovedUnaryInstructionsDefault(vector<std::shared_ptr<QuGate> > &finalProgram, int nextNonUnaryIndex);

    int insertRemovedUnaryInstructionsDAG(vector<std::shared_ptr<QuGate> > &finalProgram, int nextNonUnaryIndex);
};

#endif //UCFQUSIM_QUSMARTSWAPPER_H
