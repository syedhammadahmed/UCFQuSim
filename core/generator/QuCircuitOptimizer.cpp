//
// Created by YUSHAH on 12/4/2020.
//

#include "QuCircuitOptimizer.h"


bool QuCircuitOptimizer::isClearInBetween(int left, int right, vector<QuGate *> &finalProgram) {
    const auto& operands = finalProgram[left]->getArgIndex();
    for (int i = left + 1; i < right; i++) {
        QuGate* currentGate = finalProgram[i];
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

bool QuCircuitOptimizer::areCancellable(int left, int right, vector<QuGate*>& finalProgram) {
    QuGate* leftGate = finalProgram[left];
    QuGate* rightGate = finalProgram[right];
    if (!leftGate->isCancelled() && leftGate->isDitto(rightGate)) {
        if (isClearInBetween(left, right, finalProgram)) {
            return true;
        }
    }
    return false;
}

int QuCircuitOptimizer::performCNOTCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
    int left = -1, right = -1;
    int count = 0;
    for (left = 0; left < finalProgram.size() - 1; left++) {
        for (right = left + 1; right < finalProgram.size(); right++) {
            if(!finalProgram[left]->isUnary() && !finalProgram[right]->isUnary()) {
                if (areCancellable(left, right, finalProgram)) {
                    finalProgram[left]->setCancelled(true);
                    finalProgram[right]->setCancelled(true);
                    count += 2;
                }
            }
        }
    }
    return count;
}

int QuCircuitOptimizer::performUnaryCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent single qubits and check in-between
    int cancellations = 0;
    cout << "program size (before): " << finalProgram.size() << endl;
    int i = 0, j = 0, g = finalProgram.size();
    bool cancelled = false;
    // todo identity opearation to cancel out
    while(i < g){
        bool isUnary1 = finalProgram[i]->getCardinality() == 1;
        int arg1 = finalProgram[i]->getArgAtIndex(0);
        if (isUnary1) {
            j = i + 1;
            if(j < g){
                bool isUnary2 = (finalProgram[j]->getCardinality() == 1);
                int arg2 = finalProgram[j]->getArgAtIndex(0);
                cancelled = false;
                while (j < g) {
                    if(isUnary2){
                        if(arg1 == arg2){
                            cancelled = true;
                        }
                    }
                    else {
                        int arg3 = finalProgram[j]->getArgAtIndex(1);
                        if((arg2 != arg1) && (arg3 != arg1)) {
                            j++;
                            if (j < g) {
                                isUnary2 = (finalProgram[j]->getCardinality() == 1);
                                arg2 = finalProgram[j]->getArgAtIndex(0);
                            }
                            continue;
                        }
                        else {
                            i = j;
                            break;
                        }

                    }
                    if(cancelled) {
                        finalProgram.erase(finalProgram.begin() + j);
                        cancellations++;
                        g--;
                    } else
                        j++;
                    if (j < g) {
                        isUnary2 = (finalProgram[j]->getCardinality() == 1);
                        arg2 = finalProgram[j]->getArgAtIndex(0);
                    }
                }
            }
        }
        else {
            cancelled = false;
        }
        if(cancelled)
            i = j + 1;
        else
            i++;
        cout << i << endl;
    }
    cout << "program size (after): " << finalProgram.size()  << " cancellations: " <<  cancellations << endl;

    return cancellations;
}