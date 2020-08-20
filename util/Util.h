//
// Created by YUSHAH on 10/21/2019.
//

#ifndef UCFQUSIM_UTIL_H
#define UCFQUSIM_UTIL_H


#include <string>
#include <vector>
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

    void permute(vector<int> a, int l, int r, vector<vector<int>> &perms);
};


#endif //UCFQUSIM_UTIL_H
