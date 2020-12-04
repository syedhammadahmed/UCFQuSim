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

    if (leftGate->isDitto(rightGate)) {
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

//int QuCircuitOptimizer::performCNOTCancellations(vector<QuGate*>& finalProgram) { // todo also check non-adjacent CNOTS and check in-between see fig 9 SOTA 2
//    int src1 = -1, target1 = -1, src2 = -1, target2 = -1, src3 = -1, target3 = -1;
//    bool cancelIt = true;
//    bool matchFound = false;
//    int i = 0, j = 0, k = 0, g = 0;
//    vector<int> cancelIndexes;
//    int size = finalProgram.size();
//    cout << "program size (before): " << finalProgram.size() << endl;
//    while(g < size){
//        while(finalProgram[i++]->getMnemonic() != "cx");
//        i--;
//
//        for(j = i + 1; j < finalProgram.size(); j++){
//            cancelIt = true;
//            matchFound = false;
//            src1 = finalProgram[i]->getArgAtIndex(0);
//            target1 = finalProgram[i]->getArgAtIndex(1);
//            if(finalProgram[j]->getMnemonic() == "cx") {
//                src2 = finalProgram[j]->getArgAtIndex(0);
//                target2 = finalProgram[j]->getArgAtIndex(1);
//
//                if (src1 == src2 && target1 == target2) {
//                    matchFound = true;
//                    cout << "match found : " << i << " " << j << endl;
//                    k = i + 1;
//                    while (k < j) {
//                        src3 = finalProgram[k]->getArgAtIndex(0);
//                        if (finalProgram[k]->getCardinality() == 1) {
//                            if (src1 == src3 || target1 == src3)
//                                cancelIt = false;
//                        } else {
//                            target3 = finalProgram[k]->getArgAtIndex(1);
//                            if (src1 == target3 || target1 == src3)
//                                cancelIt = false;
//                        }
//                        k++;
//                    }
//                }
//                if (matchFound) {
//                    if (cancelIt) {
//                        cancelIndexes.push_back(i);
//                        cancelIndexes.push_back(j);
//                        finalProgram.erase(finalProgram.begin() + i);
//                        finalProgram.erase(finalProgram.begin() + j - 1);
//                        g++;
//                        i = j - 2;
//                        break;
//                    } else {
//                        i = j;
//                    }
//                }
//            }
//            else {
//
//            }
//        }
//        cout << "iteration " << g++ << endl;
//    }
//    // todo remove cancelled CNOTS from finalProgram
//    cout << (cancelIndexes.empty()? "not cancelled!" : "cancelled!") << endl;
//    cout << "program size (after): " << finalProgram.size() << ", g = " << g << endl;
//    return cancelIndexes.size();
//}
//
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

//vector<QuGate *>::iterator QuCircuitOptimizer::findMergingPartner(vector<QuGate *>::iterator it1, vector<QuGate *>::iterator end) {
//    vector<QuGate *>::iterator it2 = it1++;
//    int quBit1 = (*it1)->getArgAtIndex(0);
//    bool isUnary2;
//    int quBit2 = -1;
//    do{
//        isUnary2 = (*it2)->getCardinality() == 1;
//        quBit2 = (*it2)->getArgAtIndex(0);
//        if (quBit1 == quBit2){
//            if (isUnary2) {
//                break;
//            }
//            else {
//                int quBit2Target = (*it2)->getArgAtIndex(1);
//                it2 = it1;
//                break;
//            }
//        }
//        it2++;
//    } while (quBit1 != quBit2 && it2 != end);
//    return it2;
//}
