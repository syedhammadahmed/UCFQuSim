//
// Created by SHA on 9/13/20.
//

#ifndef UCFQUSIM_QUCIRCUITLAYERMANAGER_H
#define UCFQUSIM_QUCIRCUITLAYERMANAGER_H


#include <bits/unordered_map.h>

class QuCircuitLayerManager {

private:
    static QuCircuitLayerManager* instance;
    int rows; // total qubits
    int cols; // total instructions
    vector<vector<int>> simpleGrid;
    vector<std::shared_ptr<QuGate>> instructions;
    vector<int> quBitRecentLayer;
    int layer;

    unordered_map<int, vector<int>> layerwiseInstructionsMap;

    QuCircuitLayerManager(vector<std::shared_ptr<QuGate> > instructions, int rows);
    QuCircuitLayerManager();

public:
//    static QuCircuitLayerManager* getInstance();

    int getLayerForNewGate(vector<int> quBits, int operands);

    void addSimple(std::shared_ptr<QuGate> gate, int depth, int instructionNo);

    void buildGrid();

    bool somethingInBetween(vector<int> quBits, int operands, int layer);

    bool somethingInBetween(int row1, int row2, int layer);

    void printSimpleGrid();

    vector<int> getNextSourceInstructionIds();

    int getRows() const;

    void setRows(int rows);

    const vector<std::shared_ptr<QuGate>> getInstructions() const;

    void setInstructions(const vector<std::shared_ptr<QuGate>> instructions);

    void init();

    static QuCircuitLayerManager *getInstance(vector<std::shared_ptr<QuGate> > instructions, int rows);

    void removeInstruction(int id);

    void updateSimpleGrid(int id);

    static void deleteInstance();

    vector<int> getFirstKInstructionIds(int k);

    bool isNewInsturction(int instructionId,  vector<int> firstKIds);

    std::shared_ptr<QuGate> getInstructionById(int id);

    unordered_map<int, std::shared_ptr<QuGate>> getInstructionMap();
    unordered_map<int, vector<int>> buildLayerwiseInstructionsMap();

    void removeAllInstructions(vector<int> ids);
};


#endif //UCFQUSIM_QUCIRCUITLAYERMANAGER_H
