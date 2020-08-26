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
    vector<string> perms;
    vector<bool> allocated;
    vector<pair<int, int>> couples;
    vector<vector<int>> couplingMapAdjList;

public:
    QuMappingInitializer(int n);

    QuMappingInitializer();

    void initGenerator();
    void generateMappings();
    QuMapping getNextMapping();

    const vector<string> &getPerms();

    void initGenerator(int n);

    const int getPermCount();

    static const int TOTAL_PERM;

    vector<QuMapping> generateSmartMappings(vector<pair<int, int>> restrictionPairs, QuArchitecture& quArchitecture);

    QuMapping getNextMapping(vector<int> restrictionPairs);

    QuMapping getNextMapping(vector<pair<int, int>> restrictionPairs);

    void restrict(QuMapping &mapping, int first, int second);

    bool isAllocated(int logicalQuBit);

    pair<int, int> getCouple(int first, int second);

    void makeCouples(QuArchitecture &quArchitecture);

    void removeAdjacents(int physicalQuBit);

    int findNearest(QuMapping &mapping, int logicalQuBit);
};


#endif //UCFQUSIM_QUMAPPINGINITIALIZER_H
