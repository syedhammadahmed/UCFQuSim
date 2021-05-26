//
// Created by SHA on 9/27/19.
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
    virtual ~QuArchitecture();

    void init();

    // adds a CNOT constraint from src qubit to dest qubit
    void addConstraint(int src, int dest);
    void addConstraintsQX2();
    void addConstraintsQX3();
    void addConstraintsQX4();
    void addConstraintsQX5();

    bool isAdjacent(int src, int dest); // independent of cnot direction
    bool isCompatable(int src, int dest); // if src=>dest (cnot constraint)

    void makeSourceFrequencyPriorityList();
    void makeTargetFrequencyPriorityList();
    void makeCommonFrequencyPriorityLists();
    // remove source and sink qubits so that qubits having both in-degree and out-degree remain
    void removeSrcQubits(vector<pair<int, int>>& frequencies);
    void removeSinkQubits(vector<pair<int, int>>& frequencies);

    void printCouplingMatrix();
    friend std::ostream &operator<<(std::ostream &os, const QuArchitecture &architecture);

    // getters and setters
    int getN();
    int **getCouplingMap() const;
    const vector<pair<int, int>> getSrcFreqPriorityList() const;
    const vector<pair<int, int>> getTargetFreqPriorityList() const;
    const vector<pair<int, int>> getCommonSrcFreqPriorityList() const;
    const vector<pair<int, int>> getCommonTargetFreqPriorityList() const;

    vector<int> getPhysicalQubitsList();

};

#endif //UCFQUSIM_QUARCHITECTURE_H
