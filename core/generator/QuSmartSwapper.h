//
// Created by hammad on 11/19/19.
//

#ifndef UCFQUSIM_QUSMARTSWAPPER_H
#define UCFQUSIM_QUSMARTSWAPPER_H


#include "QuSwapStrategy.h"

class QuSmartSwapper: public QuSwapStrategy {
private:
    QuMapping initialMapping;
    vector<vector<QuMapping>> instructionWiseMappings;  // instructionWiseMappings[0] = mappings for instruction 0 (1st instruction)
    int perInstructionMappingCounter;
    vector<QuGate*> nonUnaryInstructions;

    void printSwapPath(vector<int> swapPath);

public:
    QuSmartSwapper(QuCircuit &circuit);

    int findTotalSwaps(QuArchitecture& quArchitecture) override;
    int findSwapsFor1Instruction(QuGate *quGate, int **couplingMap) override;
    vector<int> swapAlongPath(int* parent, int source, int destination) override;
    QuMapping getCurrentMapping() override;

//    void findAllMappings(int src, int dest, QuCircuit *circuit);
    void printMappings();

    vector<QuMapping> getAllMappingsForCurrentInstruction();

    vector<QuMapping> findAllMappingsFromPermutations(QuMapping& mapping, vector<int> sequence);

    void insertSwapGates(int source, int destination);

    vector<QuGate*> removeUnaryInstructions();
};


#endif //UCFQUSIM_QUSMARTSWAPPER_H
