//
// Created by YUSHAH on 7/24/2020.
//

#ifndef UCFQUSIM_QUMAPPINGINITIALIZER_H
#define UCFQUSIM_QUMAPPINGINITIALIZER_H


#include <core/QuMapping.h>

class QuMappingInitializer {
private:
    int n; // # of qubits
    int count; // permutation count
    QuMapping defaultMapping;
//    vector<string> perms;
    vector<vector<int>> perms;
    vector<bool> allocated;
    vector<pair<int, int>> couples;
    vector<vector<int>> couplingMapAdjList;
    QuMapping restrictedMapping;


public:
    static int TOTAL_PERM;

    QuMappingInitializer(int n);
    QuMappingInitializer();

    void initGenerator();
    void generateMappings();
    QuMapping getNextMapping();
    const vector<string> &getPerms();

    void initGenerator(int n);

    const int getPermCount();

    vector<QuMapping> generateSmartMappings(vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture);

    QuMapping getNextMapping(vector<pair<int, int>> restrictionPairs);

    void restrict(int first, int second);

    bool isAllocated(int logicalQuBit);

    pair<int, int> getCouple(int first, int second);

    void makeCouples(QuArchitecture &quArchitecture);

    void removeAdjacents(int physicalQuBit);

    int findNearest(int logicalQuBit);

    bool pred(QuMapping &a, QuMapping &b);
};


#endif //UCFQUSIM_QUMAPPINGINITIALIZER_H
