//
// Created by SHA on 9/26/19.
//

#include "QuGate.h"
#include "util/Util.h"

QuGate::QuGate() : depth(0), isElementary(false), cardinality(1), symbol("@"), printIndex(0) {
    for(int i=0; i<cardinality; i++)
        argIndex.push_back(-1);
}

//QuGate::QuGate(int depth) : depth(depth), isElementary(false), cardinality(1) {
//}
//QuGate::QuGate(int depth, bool isElementary) : depth(depth), isElementary(isElementary), cardinality(1) {
//}
//QuGate::QuGate(int depth, bool isElementary, int cardinality) : depth(depth), isElementary(isElementary), cardinality(cardinality) {
//}
QuGate::QuGate(int cardinality, string symbol, string mnemonic) : cardinality(cardinality), symbol(symbol), printIndex(0), mnemonic(mnemonic) {
    for(int i=0; i<cardinality; i++)
        argIndex.push_back(-1);
}

QuGate::QuGate(int cardinality, string symbol, string mnemonic, int printIndex) : cardinality(cardinality), symbol(symbol), printIndex(printIndex), mnemonic(mnemonic) {
    for(int i=0; i<cardinality; i++)
        argIndex.push_back(-1);
}

QuBit& QuGate::operator[](int i) {
    return *arg[i];
}

QuGate::~QuGate() {
//    delete [] argIndex;
}

std::string QuGate::getSymbol() const {
    return symbol;
}


int QuGate::getCardinality() const {
    return cardinality;
}

int QuGate::getPrintIndex() const {
    return printIndex;
}

void QuGate::setPrintIndex(int printIndex) {
    this -> printIndex = printIndex;
}

std::string QuGate::getPrintSymbol() {
    if (printIndex >= 1)
        return printSymbol[printIndex];
    return symbol;
}

const string &QuGate::getMnemonic() const {
    return mnemonic;
}

void QuGate::setMnemonic(const string &mnemonic) {
    QuGate::mnemonic = mnemonic;
}

std::ostream &operator<<(std::ostream &os, const QuGate& quGate) {
    string instruction = Util::toLower(quGate.getMnemonic()) + " q[" + to_string(quGate.getArgAtIndex(0))+ "]";
    if (quGate.getCardinality() > 1)
        instruction += ", q[" + to_string(quGate.getArgAtIndex(1)) + "]";
    instruction += ";";
    os << instruction;
    return os;
}

bool QuGate::isUnary() {
    return (cardinality == 1);
}

int QuGate::getArgAtIndex(int index) const {
    if(index >= 0 && index < cardinality)
        return argIndex[index];
    return -1;
}

void QuGate::setArgAtIndex(int index, int val) {
    if(index >= 0 && index < cardinality)
        argIndex[index] = val;
}

const vector<int> &QuGate::getArgIndex() const {
    return argIndex;
}

void QuGate::setArgIndex(const vector<int> &argIndex) {
    QuGate::argIndex = argIndex;
}

string QuGate::getTheta() const {
    return theta;
}

void QuGate::setTheta(string theta) {
    this->theta = theta;
}

int QuGate::getGateId() const {
    return gateId;
}

void QuGate::setGateId(int gateId) {
    this->gateId = gateId;
}

// ignores order of qubits
bool QuGate::operator==(QuGate& arg) {
    return ((this->argIndex[0] == arg.argIndex[0]) && (this->argIndex[1] == arg.argIndex[1])) || ((this->argIndex[0] == arg.argIndex[1]) && (this->argIndex[1] == arg.argIndex[0]));
}

// ignores order of qubits
bool QuGate::operator!=(QuGate& arg) {
    return !((*this) == arg);
}

bool QuGate::isDitto(shared_ptr<QuGate> arg) {
    return ((this->argIndex[0] == arg->argIndex[0]) && (this->argIndex[1] == arg->argIndex[1]));
}

bool QuGate::hasAnyOfArgs(const vector<int> argIndex) {
    for (int i = 0; i < argIndex.size(); ++i) {
        if (this->argIndex[0] == argIndex[i])
            return true;
        if (cardinality > 1) {
            if (this->argIndex[1] == argIndex[i])
                return true;
        }
    }
    return false;
}

bool QuGate::isCancelled() const {
    return cancelled;
}

void QuGate::setCancelled(bool cancelled) {
    QuGate::cancelled = cancelled;
}

//void QuGate::operator=(QuGate &arg) {
//    cardinality = arg.cardinality;
//    for(int i=0; i<cardinality; i++)
//        argIndex[i] = arg.argIndex[i];
//    mnemonic = arg.mnemonic;
//}
