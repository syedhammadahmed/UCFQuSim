//
// Created by YUSHAH on 12/4/2020.
//

#include <util/Util.h>
#include "QuCircuitOptimizer.h"
#include "QuCircuitGenerator.h"


shared_ptr<QuGate>** QuCircuitOptimizer::grid = NULL;

bool QuCircuitOptimizer::isClearInBetween(int left, int right, vector<shared_ptr<QuGate> > &finalProgram) {
    const auto& operands = finalProgram[left]->getArgIndex();
    for (int i = left + 1; i < right; i++) {
        shared_ptr<QuGate> currentGate = finalProgram[i];
        if(currentGate->isUnary()){
            if(currentGate->hasAnyOfArgs(operands)) {
                return false;
            }
        }
        else {
            int source = operands[0];
            int target = operands[1];
            if ((currentGate->getArgAtIndex(0) == target) || (currentGate->getArgAtIndex(1) == source))
                return false;
        }
    }
    return true;
}

bool QuCircuitOptimizer::areCancellable(int left, int right, vector<shared_ptr<QuGate>>& finalProgram) {
    auto& leftGate = finalProgram[left];
    auto& rightGate = finalProgram[right];
    if (!leftGate->isCancelled() && leftGate->isDitto(rightGate)) {
        if (isClearInBetween(left, right, finalProgram)) {
            return true;
        }
    }
    return false;
}

//int QuCircuitOptimizer::performCNOTCancellations(int n, vector<shared_ptr<QuGate>>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
//    int cancellations = 0;
//
//    QuCircuitGenerator quCircuitGenerator(n, finalProgram);
//    quCircuitGenerator.buildGrid();
//    auto& circuit = quCircuitGenerator.getCircuit();
//    grid = circuit.getGrid();
//
//    for (int i = 0; i < n; ++i) {
//        int s0, s1, t0, t1;
//        bool first = true;
//        bool more = false;
//        int j = 0;
//        while (i < n && j < finalProgram.size()) {
//            if (i==2)
//                cout << "";
//            auto &gate = grid[i][j];
//            if (gate == nullptr) {
//                j++;
//                if (j==finalProgram.size()) {
//                    break;
//                }
//                continue;
//            }
//            else if (!gate->isCancelled() && gate->isBinary()) {
//                if (first) {
//                    s0 = (*gate)[0];
//                    t0 = (*gate)[1];
//                    first = false;
//                    gate->setCancelled(true);
//                    grid[t0][j]->setCancelled(true);
//                } else {
//                    s1 = (*gate)[0];
//                    t1 = (*gate)[1];
//                    if (s0 == s1 && t0 == t1) {
//                        cancellations += 2;
//                        first = true;
//                        gate->setCancelled(true);
//                        grid[t0][j]->setCancelled(true);
//                    }
//                    else{
//                        more = true;
//                    }
//                }
//            }
//            else { // isUnary
//                if (!first) {
//                    s1 = (*gate)[0];
//                    if (s0 == s1) {
//                        first = true;
//                    }
//                }
//            }
//            j++;
//            if (more && j == finalProgram.size())
//                j = 0;
//        }
//    }
//    return cancellations;
//}

int QuCircuitOptimizer::performCNOTCancellations(int n, vector<shared_ptr<QuGate>>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
    int left = -1, right = -1;
    int cancellations = 0;
    for (left = 0; left < finalProgram.size() - 1; left++) {
        for (right = left + 1; right < finalProgram.size(); right++) {
            if(!finalProgram[left]->isUnary() && !finalProgram[right]->isUnary()) {
                if (areCancellable(left, right, finalProgram)) {
                    finalProgram[left]->setCancelled(true);
                    finalProgram[right]->setCancelled(true);
                    cancellations += 2;
                }
            }
        }
    }
    return cancellations;
}
//
// todo also check non-adjacent single qubits and check in-between
int QuCircuitOptimizer::performUnaryCancellations(int n, vector<shared_ptr<QuGate>>& finalProgram) {
    int cancellations = 0;
    QuCircuitGenerator quCircuitGenerator(n, finalProgram);
    quCircuitGenerator.buildGrid();
    auto& circuit = quCircuitGenerator.getCircuit();
    grid = circuit.getGrid();
//    Util::setVerbose();
//    circuit.printGrid();
//    Util::resetVerbose();

    for (int i = 0; i < n; ++i) {
        int unarySoFar = 0;
        for (int j = 0; j < finalProgram.size(); ++j) {
            if (grid[i][j] == NULL)
                continue;
            if (grid[i][j]->isBinary()) {
                if (unarySoFar > 0)
                    cancellations += unarySoFar - 1;
                unarySoFar = 0;
            }
            else { // isUnary
                unarySoFar++;
            }
        }
        if (unarySoFar > 1)
            cancellations += unarySoFar - 1;
    }
    return cancellations;
}

