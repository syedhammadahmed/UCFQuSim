//
// Created by hammad on 7/16/20.
//

#ifndef UCFQUSIM_ALLSHORTESTPATHSFINDER_H
#define UCFQUSIM_ALLSHORTESTPATHSFINDER_H

#include <list>
using namespace std;

class AllShortestPathsFinder {

private:
    int **graph;
    int n;
    list<int> *adj;

    void convertAdjMatrixToAdjList();

public:
    AllShortestPathsFinder(int **graph, int n);

    // Prints all paths from 'src' to 'dest'
    void printAllPaths(int src, int dest);

    // A recursive function to print all paths from src to dest.
    // visited[] keeps track of vertices in current path.
    // path[] stores actual vertices
    // path_index is current index in path[]
    void printAllPathsUtil(int src, int dest, bool visited[], int path[], int &path_index);

    int* findSingleSourceAllShortestPaths(int src, int dest);



};


#endif //UCFQUSIM_ALLSHORTESTPATHSFINDER_H
