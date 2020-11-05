//
// Created by YUSHAH on 10/21/2019.
//

#ifndef UCFQUSIM_UTIL_H
#define UCFQUSIM_UTIL_H


#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

class Util {
public:
    static bool verbose;
    static string FILE_EXTENSION;
    static string toLower(string str);
    static double timeIt(bool isEnd);
    static void printPath(vector<int> path);

    static void permute(string a, int l, int r, vector<string> &perms);

    static long long factorial(int n);
    static void print(string str);
    static void println(string str);

    static string pathToString(vector<int> path);

    static void parseMappingId(string mappingId, int &parentProgramCounter, int &parentMappingCounter);

    static void permute(vector<int> a, int l, int r, vector<vector<int>> &perms);
    static bool sortBySecDesc(const pair<int,int> &a, const pair<int,int> &b);

    static bool sortByFirstAsc(const pair<int,int> &a, const pair<int,int> &b);
    static bool intersectionCompare(pair<int, int> &a, pair<int, int> &b);

    static vector<vector<int>> getNRandomPermutations(int n, vector<int> result);

//    static vector<int> getMappingIndexSamples(int threshold, unsigned long size);

    static unordered_set<int> getNRandomIndexes(int sampleSize, int rangeUpperBound);

    static void setVerbose();
    static void resetVerbose();
};


#endif //UCFQUSIM_UTIL_H
