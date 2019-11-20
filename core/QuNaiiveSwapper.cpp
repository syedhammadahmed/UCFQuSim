//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include "QuNaiiveSwapper.h"

int QuNaiiveSwapper::findSwaps(QuGate *quGate, int **couplingMap, QuCircuit* circuit) {
    ShortestPathFinder spf(couplingMap, circuit->getRows());
    int* parent = NULL;
    int inputs = quGate -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    int physicalIndex1 = circuit->getMapping().getPhysicalBit(quBitIndexes[0]);
    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
    if(inputs == 2){
        int physicalIndex2 = circuit->getMapping().getPhysicalBit(quBitIndexes[1]);
//        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
        circuit->getSwapPath().clear();
        circuit->swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        circuit->getMapping().fixMappings(physicalIndex1, circuit->getSwapPath());
//        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        swaps = circuit->getSwapPath().size();
//        if(swaps == 0)
//            cout << "No swap required!" << endl;
//        printMappings();
    }
    circuit->getInstructionsV1().push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}
