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
    static string FILE_EXTENSION;
    static string toLower(string str);
    static double timeIt(bool isEnd);
    static void printPath(vector<int> path);

    static void permute(string a, int l, int r, vector<string> &perms);

    static long long factorial(int n);
};


#endif //UCFQUSIM_UTIL_H
