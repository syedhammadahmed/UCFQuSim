//
// Created by SHA on 10/21/2019.
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
#include "QuMapping.h"

using namespace std;

class Util {
public:
    static bool verbose;
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

    static int findMin(vector<int> arg);

    //https://www.gormanalysis.com/blog/random-numbers-in-cpp/#sampling-without-replacement
    static void randomSampling(vector<QuMapping> &population, vector<QuMapping> &sample, int sampleSize);

    static vector<int> makeMappingVector();

    static void printPairs(vector<pair<int, int>> pairs);

    static vector<string> tokenize(string str, string delim = " ");

    static void removeDuplicates(vector<int> &list);
};


#endif //UCFQUSIM_UTIL_H
