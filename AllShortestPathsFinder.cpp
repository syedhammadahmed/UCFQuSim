//
// Created by hammad on 7/16/20.
//

#include "AllShortestPathsFinder.h"
#include <iostream>
#include <list>

using namespace std;

// Prints all paths from 's' to 'd'
void AllShortestPathsFinder::printAllPaths(int s, int d, int swaps)
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
    printAllPathsUtil(s, d, visited, path, path_index);
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void AllShortestPathsFinder::printAllPathsUtil(int u, int d, bool visited[],
                              int path[], int &path_index)
{
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d)
    {
        for (int i = 0; i<path_index; i++)
            cout << path[i] << " ";
        cout << endl;

    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (!visited[*i])
                printAllPathsUtil(*i, d, visited, path, path_index);
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
}


int *AllShortestPathsFinder::findSingleSourceAllShortestPaths(int src, int dest) {
    printAllPaths(src, dest);
    return nullptr;
}


AllShortestPathsFinder::AllShortestPathsFinder(int **graph, int n) : graph(graph), n(n) {
    adj = new list<int>[n];
    convertAdjMatrixToAdjList();
}

void AllShortestPathsFinder::convertAdjMatrixToAdjList() {
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++) {
            if(graph[i][j] != 0)
                adj[i].push_back(j);
        }
    }
}
