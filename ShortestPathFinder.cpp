//
// Created by YUSHAH on 10/17/2019.
//

#include <iostream>
#include <cstdlib>
#include "ShortestPathFinder.h"


ShortestPathFinder::ShortestPathFinder(int **graph, int n): graph(graph), n(n) {
    dist = new int*[n];
    for (int i = 0; i < n; i++)
        dist[i] = new int[n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dist[i][j] = graph[i][j];

    dist1D = new int[n];
    parent = new int[n];

}

// Solves the all-pairs shortest path
// problem using Floyd Warshall algorithm
int** ShortestPathFinder::findShortestPaths()
{
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = abs(dist[i][j]);
        }
    }

    // For each intermediate
    for (int k = 0; k < n; k++)
    {
        // For each source
        for (int i = 0; i < n; i++)
        {
            // For each destination
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
//    printSPGrid();
    return dist;
}


void ShortestPathFinder::swap(int& num1, int& num2){
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

void ShortestPathFinder::printSPGrid(){
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            std::cout << dist[i][j] << " ";
        std::cout << std::endl;
    }
}

int* ShortestPathFinder::findSingleSourceShortestPaths(int **graph, int source){
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = abs(graph[i][j]);
        }
    }

    bool* shortestPathSet = new bool[n];

    // Initialize all distances as INFINITE and shortestPathSet[] as false
    for (int i = 0; i < n; i++)
    {
        parent[i] = -1;
        dist1D[i] = INT16_MAX;
        shortestPathSet[i] = false;
    }

    dist1D[source] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < n - 1; count++)
    {
        // Pick the minimum distance
        // vertex from the set of
        // vertices not yet processed.
        // u is always equal to src
        // in first iteration.
        int u = findMin(dist1D, shortestPathSet);

        // Mark the picked vertex
        // as processed
        shortestPathSet[u] = true;

        // Update dist value of the
        // adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < n; v++)

            // Update dist[v] only if is
            // not in sptSet, there is
            // an edge from u to v, and
            // total weight of path from
            // src to v through u is smaller
            // than current value of
            // dist[v]
            if (!shortestPathSet[v] && graph[u][v] &&
                    dist1D[u] + graph[u][v] < dist1D[v])
            {
                parent[v] = u;
                dist1D[v] = dist1D[u] + graph[u][v];
            }


    }

    return parent;
}


// A utility function to find the
// vertex with minimum distance
// value, from the set of vertices
// not yet included in shortest
// path tree
int ShortestPathFinder::findMin(int* dist, bool* shortestPathSet)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (shortestPathSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}