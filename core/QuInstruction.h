//
// Created by YUSHAH on 10/19/2019.
//

#ifndef UCFQUSIM_QUINSTRUCTION_H
#define UCFQUSIM_QUINSTRUCTION_H

#include <string>

using namespace std;

class QuInstruction {
private:
    string mnemonic;
    int argCount;
    int* argValues;

public:
    QuInstruction(const string mnemonic, const int argCount, const int *argValues);

    const string &getMnemonic() const;

    void setMnemonic(const string &mnemonic);

    int getArgCount() const;

    void setArgCount(int argCount);

    int *getArgValues() const;

    void setArgValues(int *argValues);

};


#endif //UCFQUSIM_QUINSTRUCTION_H
