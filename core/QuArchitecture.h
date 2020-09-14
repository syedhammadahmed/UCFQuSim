//
// Created by hammad on 9/27/19.
//

#ifndef UCFQUSIM_QUARCHITECTURE_H
#define UCFQUSIM_QUARCHITECTURE_H


#include <ostream>
#include <vector>
using namespace std;

class QuArchitecture {
private:
    int n; // # of physical quBits
    int** couplingMap;
    vector<pair<int, int>> srcFreqPriorityList;
    vector<pair<int, int>> targetFreqPriorityList;
    vector<pair<int, int>> commonSrcFreqPriorityList;
    vector<pair<int, int>> commonTargetFreqPriorityList;
public:
    QuArchitecture(int n);
    void addConstraint(int src, int dest);
    void addConstraintsQX3();

    friend std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture);
    // adds a CNOT constraint from src qubit to dest qubit

    int getN();

    int **getCouplingMap() const;

    virtual ~QuArchitecture();

    void addConstraintsQX5();

    void printCouplingMatrix();
    bool isAdjacent(int src, int dest);

    bool isCompatable(int src, int dest);

    void makeSourceFrequencyPriorityList();
    void makeTargetFrequencyPriorityList();

    const vector<pair<int, int>>& getSrcFreqPriorityList() const;
    const vector<pair<int, int>>& getTargetFreqPriorityList() const;

    const vector<pair<int, int>> getCommonSrcFreqPriorityList() const;
    const vector<pair<int, int>> getCommonTargetFreqPriorityList() const;

    void makeCommonFrequencyPriorityLists();

    bool comp(pair<int, int> &a, pair<int, int> &b);

    void removeSrcQubits(vector<pair<int, int>>& frequencies);
    void removeSinkQubits(vector<pair<int, int>>& frequencies);
};


#endif //UCFQUSIM_QUARCHITECTURE_H
