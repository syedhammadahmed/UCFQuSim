//
// Created by hammad on 11/19/19.
//

#include <ShortestPathFinder.h>
#include "QuNaiiveSwapper.h"
#include "QuGateFactory.h"

int QuNaiiveSwapper::findSwapsFor1Instruction(QuGate *quGate, int **couplingMap) {
    ShortestPathFinder spf(couplingMap, circuit.getRows());
    int* parent = NULL;
    int cardinality = quGate -> getCardinality(); // # of qubits in a gate
    int* quBitIndexes = quGate -> getArgIndex(); // logical qubit index values
    int swaps = 0;
    QuMapping& mapping = circuit.getMapping();
    vector<int>& swapSequence = swapPath;
//    vector<int>& swapSequence = circuit.getSwapPath();

    int physicalIndex1 = mapping.getPhysicalBit(quBitIndexes[0]);
    parent = spf.findSingleSourceShortestPaths(couplingMap, physicalIndex1);
    if(cardinality == 2){
        int physicalIndex2 = mapping.getPhysicalBit(quBitIndexes[1]);
//        cout << "Finding swaps from " << quBitIndexes[0] << " to " << quBitIndexes[1] << " : " << endl;
        swapSequence.clear();
        swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
//        circuit.swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        mapping.fixMappings(physicalIndex1, swapSequence);
//        swaps = swapAlongPath(parent, physicalIndex1, parent[physicalIndex2]);
        swaps = swapSequence.size();
//        if(swaps == 0)
//            cout << "No swap required!" << endl;
//        printMappings();
    }
    circuit.getInstructionsV1().push_back(quGate); // new program which includes swap gates for CNOT-constraint satisfaction
    return swaps;
}

QuNaiiveSwapper::QuNaiiveSwapper(QuCircuit &circuit) : QuSwapStrategy(circuit) {}

int QuNaiiveSwapper::findTotalSwaps(QuArchitecture& quArchitecture)  {
    int total = 0;

    for(QuGate* quGate: circuit.getInstructions()){
        cout << "Instruction #: " << programCounter++ << endl;
        total += findSwapsFor1Instruction(quGate, quArchitecture.getCouplingMap());
    }
    return total;
}



QuMapping QuNaiiveSwapper::getCurrentMapping() {
    return circuit.getMapping();
}

vector<int> QuNaiiveSwapper::swapAlongPath(int* parent, int source, int destination) {
    int count = 0;

    if (parent[destination] != -1){
//        count = swapAlongPath(parent, mapping.getPhysicalBit(mapping.getLogicalMapping(source)), mapping.getPhysicalBit(mapping.getLogicalMapping(parent[destination]))) + 1;
        swapAlongPath(parent, source, parent[destination]);
//        count = swapAlongPath(parent, source, parent[destination]) + 1;
//        cout << "(" << source << " | ) " << destination << " -> ";
//        cout << "Swap: <";
//        cout << mapping.getLogicalMapping(source) << ", "
//             << mapping.getLogicalMapping(destination) << ">" << endl;
        swapPath.push_back(destination);
//        mapping.quSwap(source, destination);
//        int temp = mapping.getLogicalMapping(parent[destination]);
//        mapping.setLogicalMapping(parent[destination], mapping.getLogicalMapping(destination));
//        mapping.setLogicalMapping(destination, temp);

        QuGate* swapGate = QuGateFactory::getQuGate("SWAP");
        int* args = swapGate -> getArgIndex();
        args[0] = getCurrentMapping().getLogicalMapping(source);
        args[1] = getCurrentMapping().getLogicalMapping(destination);
        circuit.getInstructionsV1().push_back(swapGate);
    }
    return swapPath;
}