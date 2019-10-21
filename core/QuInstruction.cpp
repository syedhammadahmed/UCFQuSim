//
// Created by YUSHAH on 10/19/2019.
//

#include "QuInstruction.h"

const string &QuInstruction::getMnemonic() const {
    return mnemonic;
}

void QuInstruction::setMnemonic(const string &mnemonic) {
    QuInstruction::mnemonic = mnemonic;
}

int QuInstruction::getArgCount() const {
    return argCount;
}

void QuInstruction::setArgCount(int argCount) {
    QuInstruction::argCount = argCount;
}

int *QuInstruction::getArgValues() const {
    return argValues;
}

void QuInstruction::setArgValues(int *argValues) {
    QuInstruction::argValues = argValues;
}

QuInstruction::QuInstruction(const string mnemonic, const int argCount, const int *argValues) : mnemonic(mnemonic), argCount(argCount),
                                                                                                argValues(NULL) {

}
