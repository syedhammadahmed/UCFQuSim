//
// Created by hammad on 7/16/20.
//

#include "AllShortestPathsFinder.h"
#include <iostream>
#include <list>
#include <vector>
#include <util/Util.h>

using namespace std;

// Prints all paths from 's' to 'd'
void AllShortestPathsFinder::printAllPaths(int src, int dest, int swaps)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[n];

    // Create an array to store paths
    int *path = new int[n];
    int path_index = 0; // Initialize path[] as empty

    // Initialize all vertices as not visited
    for (int i = 0; i < n; i++)
        visited[i] = false;

    // Call the recursive helper function to print all paths
    printAllPathsUtil(src, dest, visited, path, path_index, swaps);

    delete [] visited;
    delete [] path;
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices
// path_index is current index in path[]
void AllShortestPathsFinder::printAllPathsUtil(int src, int dest, bool visited[],
                              int path[], int &path_index, int swaps)
{
    // Mark the current node and store it in path[]
    visited[src] = true;
    path[path_index] = src;
    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (src == dest)
    {
//        if(swaps == path_index-2 && path[path_index-1] == dest) { // only shortest path
        if(swaps == path_index-2) { // only shortest path
            vector<int> temp;
            for (int i = 0; i < path_index; i++){
//                cout << path[i] << " ";
                temp.push_back(path[i]);
            }
            allSwapPaths.push_back(temp);
//            cout << endl;
            swapPathCounter++;
        }
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        list<int>::iterator i;
        for (i = adj[src].begin(); i != adj[src].end(); ++i)
            if (!visited[*i])
                printAllPathsUtil(*i, dest, visited, path, path_index, swaps);
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[src] = false;
}


vector<vector<int>> AllShortestPathsFinder::findSingleSourceAllShortestPaths(int src, int dest, int swaps) {
    allSwapPaths.clear();
    string temp = to_string(src) + "," + to_string(dest);
//    if(preCalPaths.count(temp) > 0) {
//        cout << "hello";
//    }
    allSwapPaths = preCalPaths[temp];
    if(allSwapPaths.empty()) {
//        Util::println("Getting precalculated SPs...");
//    }
//    else {
        printAllPaths(src, dest, swaps);
        preCalPaths[temp] = allSwapPaths;
    }
    return allSwapPaths;
}


AllShortestPathsFinder::AllShortestPathsFinder(int **graph, int n) : graph(graph), n(n), swapPathCounter(0) {
    adj = new list<int>[n];
    convertAdjMatrixToAdjList();
}

AllShortestPathsFinder::~AllShortestPathsFinder() {
    delete [] adj;
}

void AllShortestPathsFinder::convertAdjMatrixToAdjList() {
    for(int i=0; i<n; i++){
        adj[i].clear();
        for(int j=0; j<n; j++) {
//            if(graph[i][j] != 0 || graph[j][i] != 0){
            if(graph[i][j] != 0 || graph[j][i] != 0){
                adj[i].push_back(j);
//                adj[j].push_back(i);
            }
        }
    }
//    for(int i=0; i<n; i++)
//        adj[i].unique();
//    if(Util::verbose) {
//        for (int i = 0; i < n; i++) {
//            cout << i << ": ";
//            for (int j: adj[i]) {
//                cout << j << " ";
//            }
//            cout << endl;
//        }
//    }
}

const vector<vector<int>> &AllShortestPathsFinder::getAllSwapPaths() const {
    return allSwapPaths;
}
