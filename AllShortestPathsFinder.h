//
// Created by hammad on 7/16/20.
//

#ifndef UCFQUSIM_ALLSHORTESTPATHSFINDER_H
#define UCFQUSIM_ALLSHORTESTPATHSFINDER_H

#include <list>
#include <vector>

using namespace std;

class AllShortestPathsFinder {

private:
    int **graph;
    int n;
    list<int> *adj;
    vector<vector<int>> allSwapPaths;
    int swapPathCounter;

    void convertAdjMatrixToAdjList();

public:
    AllShortestPathsFinder(int **graph, int n);

    // Prints all paths from 'src' to 'dest'
    void printAllPaths(int src, int dest, int swaps);

    // A recursive function to print all paths from src to dest.
    // visited[] keeps track of vertices in current path.
    // path[] stores actual vertices
    // path_index is current index in path[]
    void printAllPathsUtil(int src, int dest, bool visited[], int path[], int &path_index, int swaps);

    vector<vector<int>> findSingleSourceAllShortestPaths(int src, int dest, int swaps);

    const vector<vector<int>> &getAllSwapPaths() const;
};


#endif //UCFQUSIM_ALLSHORTESTPATHSFINDER_H
