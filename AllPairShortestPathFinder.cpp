//
// Created by hammad on 9/3/20.
// ref: http://masc.cs.gmu.edu/wiki/FloydWarshall
//

#include <climits>
#include <iostream>
#include <sstream>
#include <util/Util.h>
#include "AllPairShortestPathFinder.h"

AllPairShortestPathFinder::AllPairShortestPathFinder(int **graph, int n):n(n) {
    for (int i = 0; i < n; ++i) {
        vector<int> row;
        for (int j = 0; j < n; ++j) {
            row.push_back(0);
        }
        next.push_back(row);
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                next[i][j] = j + 1;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        vector<int> row;
        for (int j = 0; j < n; ++j) {
            int val = graph[i][j];
            if (val == 1) {
//                val = 7;
                val = 1;
                next[i][j] = j;
            }
            else if (val == -1){
//                val = 11;
                val = 1;
                next[i][j] = j;
            }
            else
                val = INT16_MAX;
            if (i==j){
                val = 0;
                next[i][j] = j;
            }
            row.push_back(val);
        }
        distance.push_back(row);
    }

    findAPSPs();
}

void AllPairShortestPathFinder::findAPSPs() {
    int i, j, k;
    for (k=0;k<n;k++)
        for (i=0;i<n;i++)
            for (j=0;j<n;j++)
                if (distance[i][j] > distance[i][k] + distance[k][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    next[i][j] = next[i][k];
                }
//    for (i=0;i<n;i++)
//        for (j=0;j<n;j++)
//            distance[i][j] -= 7;
}

pair<vector<int>, int> AllPairShortestPathFinder::getSPSequence(int src, int dest) {
    this->path.clear();
    int i = src, j = dest;
    if (i != j) {
        auto u = i;
        auto v = j;
        if (Util::verbose)
            std::cout << "(" << u << " -> " << v << ", " << distance[i][j] << ", ";
        std::stringstream path;
        path << u;
        this->path.push_back(u);
        do {
            u = next[u][v];
            path << " -> " << u;
            this->path.push_back(u);
        } while (u != v);
        if (Util::verbose)
            std::cout << path.str() << ")" << std::endl;
    }
    return make_pair(path, distance[i][j]) ;
}
