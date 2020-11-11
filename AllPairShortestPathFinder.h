//
// Created by SHA on 9/3/20.
//

#ifndef UCFQUSIM_ALLPAIRSHORTESTPATHFINDER_H
#define UCFQUSIM_ALLPAIRSHORTESTPATHFINDER_H

#include <vector>
using namespace std;


class AllPairShortestPathFinder {

private:
    vector<vector<int>> distance;
    vector<vector<int>> next;
    vector<int> path;
    int n;

public:
    AllPairShortestPathFinder(int** graph, int n);
    void findAPSPs();
    pair<vector<int>, int> getSPSequence(int src, int dest);
};


#endif //UCFQUSIM_ALLPAIRSHORTESTPATHFINDER_H
