//
// Created by SHA on 10/17/2019.
//

#include <iostream>
#include <cstdlib>
#include <util/Util.h>
#include "ShortestPathFinder.h"
using namespace std;


ShortestPathFinder::ShortestPathFinder(int **graph, int n): graph(NULL), dist(NULL), dist1D(NULL), n(n), parent(NULL) {
    if(dist == NULL) {
        dist = new int*[n];
        for (int i = 0; i < n; i++)
            dist[i] = new int[n];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                dist[i][j] = graph[i][j];
    }
    this->graph = new int*[n];
    for (int i = 0; i < n; i++)
        this->graph[i] = new int[n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            this->graph[i][j] = graph[i][j];

    if(dist1D == NULL)
        dist1D = new int[n];
    if(parent == NULL)
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
    if(Util::verbose) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                std::cout << dist[i][j] << " ";
            std::cout << std::endl;
        }
    }
}

int* ShortestPathFinder::findSingleSourceShortestPaths(int **graph, int source){
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
//            if(this->graph[i][j] == 1)
//                this->graph[i][j] = 7;
//            else if (this->graph[i][j] == -1)
//                this->graph[i][j] = 11;
            this->graph[i][j] = abs(graph[i][j]);
//            cout << graph[i][j] << " ";
        }
//        cout << endl;
    }

    bool* shortestPathSet = new bool[n];

    // Initialize all distances as INFINITE and shortestPathSet[] as false
    for (int i = 0; i < n; i++)
    {
        parent[i] = -1;
        dist1D[i] = INT32_MAX;  // set to +infinity
        shortestPathSet[i] = false;
    }

    dist1D[source] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < n; count++)
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
                    dist1D[u] + this->graph[u][v] < dist1D[v])
            {
                parent[v] = u;
                dist1D[v] = dist1D[u] + this->graph[u][v];
            }


    }
    delete [] shortestPathSet;
    return parent;
}


// A utility function to find the
// vertex with minimum distance
// value, from the set of vertices
// not yet included in shortest
// path tree
int ShortestPathFinder::findMin(int* dist, bool* shortestPathSet){
    int min = INT32_MAX, minIndex;

    for (int v = 0; v < n; v++){
        if (shortestPathSet[v] == false && dist[v] <= min){
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

ShortestPathFinder::~ShortestPathFinder() {
//    cout << "~ShortestPathFinder()" << endl;
    for (int i = 0; i < n; i++)
        delete[] dist[i];
    delete[] dist;

    for (int i = 0; i < n; i++)
        delete[] graph[i];
    delete[] graph;

    delete[] dist1D;
    delete[] parent;
}

void ShortestPathFinder::setGraph(int **graph) {
    ShortestPathFinder::graph = graph;
}
