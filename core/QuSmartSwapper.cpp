//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include <iostream>
#include "QuSmartSwapper.h"


int QuSmartSwapper::findSwaps(QuGate *quGate, int **couplingMap, QuCircuit* circuit) {
    ShortestPathFinder spf(couplingMap, circuit->getRows());
    int* parent = NULL;
    int inputs = quGate -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    int physicalIndex1 = circuit->getMapping().getPhysicalBit(quBitIndexes[0]);
    vector<int>& swapSequence = circuit->getSwapPath();
    QuMapping& mapping = circuit->getMapping();

    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);

    if(inputs == 2){
        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
        swapSequence.clear();
        circuit->swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        mapping.fixMappings(physicalIndex1, swapSequence);

        for(int i=0; i<swapSequence.size(); i++)
            cout << mapping.getLogicalMapping(swapSequence[i]) << " ";
        cout << endl;

        int src = physicalIndex1;
        int dest = physicalIndex2;
        swapSequence.insert(swapSequence.begin(), src);
        swapSequence.push_back(dest);
        findAllMappings(src, dest, circuit);

        swaps = swapSequence.size() - 2;

    }
    circuit->getInstructionsV1().push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}


void QuSmartSwapper::findAllMappings(int src, int dest, QuCircuit *circuit) {
    vector<int>& swapSequence = circuit->getSwapPath();
    // get the path
    int totalMoves = swapSequence.size()-2;

//    for(int i=0; i<swapSequence.size(); i++)
//        cout << swapSequence[i] << " ";
//    cout << endl;

    for(int i=0; i<totalMoves+1; i++){
        QuMapping mapping(circuit->getMapping());  // todo save mappings for each permutation in instructionWiseMappings
        cout << "Permutation #: " << i+1 << endl;
        int srcMoves = totalMoves - i;
        int destMoves = i;
        for(int j=0; j<srcMoves; j++)
            cout << "Swap: <" << src << ", " << swapSequence[j+1] << ">" << endl;
        for(int j=0; j<destMoves; j++)
            cout << "Swap: <" << swapSequence[totalMoves-j] << ", " << dest << ">" << endl;
        swapSequence.pop_back();
        swapSequence.erase(swapSequence.begin());
        mapping.fixMappings(swapSequence[0], swapSequence);
        instructionWiseMappings[0].push_back(mapping); // todo need to insert for ith instruction instead of 0th
    }

}
