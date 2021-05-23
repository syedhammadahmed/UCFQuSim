//
// Created by SHA on 10/21/2019.
//

#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <core/QuMapping.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include "Util.h"
#include "Constants.h"

using namespace std;

// BobFloydAlgo
unordered_set<int> Util::getNRandomIndexes(int sampleSize, int rangeUpperBound)
{
    unordered_set<int> sample;
    default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

    for(int d = rangeUpperBound - sampleSize; d < rangeUpperBound; d++)
    {
        int t = uniform_int_distribution<>(0, d)(generator);
        if (sample.find(t) == sample.end() )
            sample.insert(t);
        else
            sample.insert(d);
    }
    return sample;
}

bool Util::verbose = false;

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

void Util::permute(vector<int> a, int l, int r, vector<vector<int>> &perms)
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

void Util::parseMappingId(string mappingId, int& parentProgramCounter, int& parentMappingCounter) {
    string delimiter = ".";
    parentProgramCounter = -1;
    parentMappingCounter = -1;
    if(!mappingId.empty() && mappingId != "*"){
        parentProgramCounter = stoi(mappingId.substr(0, mappingId.find(delimiter)));
        parentMappingCounter = stoi(mappingId.substr(mappingId.find(delimiter) + 1, mappingId.length()-mappingId.find(delimiter) + 1));
    }
}

bool Util::sortBySecDesc(const pair<int,int> &a, const pair<int,int> &b)
{
    return (a.second > b.second);
}

bool Util::sortByFirstAsc(const pair<int,int> &a, const pair<int,int> &b)
{
    return (a.first < b.first);
}

bool Util::intersectionCompare(pair<int,int> &a, pair<int,int> &b)
{
    return a.first < b.first;
//    return ((a.second != 0) && (b.second != 0) && (a.first < b.first));
}

vector<vector<int>> Util::getNRandomPermutations(int n, vector<int> result) {
    vector<vector<int>> perms(n);
    for (int i = 0; i < n; i++) {
        vector<int> perm = result;
        // Create a random device and use it to generate a random seed
        random_device myRandomDevice;
        unsigned seed = myRandomDevice();

        // Initialize a default_random_engine with the seed
        default_random_engine myRandomEngine(seed);
        shuffle(perm.begin(), perm.end(), myRandomEngine);
        perms[i] = perm;
//        for (int i = 0; i < result.size(); i++) std::cout << result[i] << " ";
//        std::cout << std::endl;
    }
    return perms;
}

void Util::setVerbose() {
    verbose = true;
}

void Util::resetVerbose() {
    verbose = false;
}

int Util::findMin(vector<int> arg) {
    if(arg.size() == 0)
        return -1;
    int min = arg[0];
    for (int i = 1; i < arg.size(); ++i) {
        if (arg[i] < min)
            min = arg[i];
    }
    return min;
}
//
//vector<int> Util::getMappingIndexSamples(int threshold, unsigned long size) {
//    unordered_set<int> samples;
//
//    // Sample and insert values into samples
//    for (int r = size - threshold; r < size; ++r) {
//        random_device myRandomDevice;
//        unsigned seed = myRandomDevice();
//
//        // Initialize a default_random_engine with the seed
//        default_random_engine myRandomEngine(seed);
//
//        int v = std::uniform_int_distribution<>(1, r)(myRandomEngine);
//        if (!samples.insert(v).second) samples.insert(r);
//    }
//
//    // Copy samples into vector
//    std::vector<int> result(samples.begin(), samples.end());
//
//    // Shuffle vector
////    std::shuffle(result.begin(), result.end(), myRandomEngine);
//
//    return result;
//}

void Util::randomSampling(vector<QuMapping>& population, vector<QuMapping>& sample, int sampleSize){
    // Create a random device and use it to generate a random seed
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();
    std::default_random_engine myRandomEngine(seed);
    std::shuffle(population.begin(), population.end(), myRandomEngine);
    sample.insert(sample.begin(), population.begin(), population.begin() + sampleSize);
}

void Util::printPairs(vector<pair<int, int>> pairs) {
    for(auto pair: pairs) {
        cout << pair.first << ", " << pair.second << endl;
    }
}

vector<int> Util::makeMappingVector() {
    vector<int> mappingVector(Constants::QX5_N);
    const int *mappingArray = Constants::HARDCODED_INIT_MAPPINGS[Constants::sqn_258];
    memcpy(&mappingVector[0], mappingArray, Constants::QX5_N*sizeof(int));
    return mappingVector;
}

vector<string> Util::tokenize(string str, string delim) {
    vector<string> tokens;

    int start = 0;
    int end = str.find(delim);
    string token;
    while (end != -1) {
        token = str.substr(start, end - start);
        token.erase(std::remove(token.begin(), token.end(), ','), token.end());
        tokens.push_back(token);
        start = end + delim.size();
        end = str.find(delim, start);
    }
    token = str.substr(start, end - start);
    tokens.push_back(token);

    return tokens;
}

void Util::removeDuplicates(vector<int>& list) {
    sort(list.begin(), list.end());
    list.erase(unique(list.begin(), list.end()), list.end());
}

void Util::createDirectoryIfNotExists(string path) {
    DIR* dir = NULL;
    string directory = path;
    dir = opendir(directory.c_str());
    if(dir == NULL)
//        _mkdir(directory.c_str());
        mkdir(directory.c_str(), 0777);
}

