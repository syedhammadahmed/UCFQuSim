//
// Created by YUSHAH on 10/17/2019.
//

#ifndef UCFQUSIM_SHORTESTPATHFINDER_H
#define UCFQUSIM_SHORTESTPATHFINDER_H


class ShortestPathFinder {
private:
    int** graph;
    int** dist;
    int* dist1D;
    int* lToP;
    int* pToL;
    int n;
    // Parent array to store shortest path tree
    int* parent;

public:
    ShortestPathFinder(int **graph, int n);
    int** findShortestPaths();
    void printSPGrid();

    int* findSingleSourceShortestPaths(int **graph, int source);

    int findMin(int *dist, bool *shortestPathSet);

//    int swapAlongPath(int *parent, int source, int destination);

    void swap(int &num1, int &num2);
};


#endif //UCFQUSIM_SHORTESTPATHFINDER_H
