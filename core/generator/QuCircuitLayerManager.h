//
// Created by hammad on 9/13/20.
//

#ifndef UCFQUSIM_QUCIRCUITLAYERMANAGER_H
#define UCFQUSIM_QUCIRCUITLAYERMANAGER_H


class QuCircuitLayerManager {

private:
    static QuCircuitLayerManager* instance;
    int rows; // total qubits
    int cols; // total instructions
    vector<vector<int>> simpleGrid;
    vector<QuGate*> instructions;
    vector<int> quBitRecentLayer;
    int layer;

    QuCircuitLayerManager(vector<QuGate *> instructions, int rows);
    QuCircuitLayerManager();

public:
//    static QuCircuitLayerManager* getInstance();

    int getLayerForNewGate(vector<int> quBits, int operands);

    void addSimple(QuGate *gate, int depth, int instructionNo);

    void buildGrid();

    bool somethingInBetween(vector<int> quBits, int operands, int layer);

    bool somethingInBetween(int row1, int row2, int layer);

    void printSimpleGrid();

    vector<int> getNextSourceInstructionIds();

    int getRows() const;

    void setRows(int rows);

    const vector<QuGate*> getInstructions() const;

    void setInstructions(const vector<QuGate*> instructions);

    void init();

    static QuCircuitLayerManager *getInstance(vector<QuGate *> instructions, int rows);

    void removeInstruction(int id);

    void updateSimpleGrid(int id);

    static void deleteInstance();

    vector<int> getFirstKInstructionIds(int k);
};


#endif //UCFQUSIM_QUCIRCUITLAYERMANAGER_H
