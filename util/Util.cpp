//
// Created by YUSHAH on 10/21/2019.
//

#include <time.h>
#include <iostream>
#include "Util.h"

using namespace std;


string Util::FILE_EXTENSION = ".qasm";
bool Util::verbose = true;

string Util::toLower(string str) {
    for(int i=0; i<str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

double Util::timeIt(bool isEnd) {
    static time_t start;
    static time_t end;
    if(!isEnd) {
        time(&start);
    }
    else {
        time(&end);
        return double(end) - double(start);
    }
    return 0.0;
}

string Util::pathToString(vector<int> path) {
    string str;
    for (int i = 0; i < path.size(); i++) {
        str += to_string(path[i]);
        if (i < path.size() - 1) str += ", ";
    }
    return str;
}

void Util::printPath(vector<int> path) {
    if(verbose) {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

void Util::permute(string a, int l, int r, vector<string> &perms)
{
    // Base case
    if (l == r)
        perms.push_back(a);
//        cout<<a<<endl;
    else
    {
        // Permutations made
        for (int i = l; i <= r; i++)
        {

            // Swapping done
            swap(a[l], a[i]);

            // Recursion called
            permute(a, l+1, r, perms);

            //backtrack
            swap(a[l], a[i]);
        }
    }
}

// ubuntu
//double Util::timeIt(bool isEnd) {
//    static time_t start;
//    static time_t end;
//    if(!isEnd) {
//        time(&start);
//    }
//    else {
//        time(&end);
//        return double(end) - double(start);
//    }
//    return 0.0;
//}

long long Util::factorial(int n){
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void Util::print(string str) {
    if(verbose)
        cout << str << " ";
}

void Util::println(string str) {
    if(verbose)
        cout << str << endl;
}
