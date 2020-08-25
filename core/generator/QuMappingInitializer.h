//
// Created by YUSHAH on 7/24/2020.
//

#ifndef UCFQUSIM_QUMAPPINGINITIALIZER_H
#define UCFQUSIM_QUMAPPINGINITIALIZER_H


#include <core/QuMapping.h>

class QuMappingInitializer {
private:
    static int n; // # of qubits
    static int count; // permutation count
    static QuMapping defaultMapping;
    static vector<string> perms;

public:
    QuMappingInitializer(int n);

    static void initGenerator();
    static void generateMappings();
    static QuMapping getNextMapping();

    static const vector<string> &getPerms();

    static void initGenerator(int n);

    static const int getPermCount();

    static const int TOTAL_PERM;
};


#endif //UCFQUSIM_QUMAPPINGINITIALIZER_H
