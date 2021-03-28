//
// Created by SHA on 9/9/20.
//

#ifndef UCFQUSIM_PRIORITYNODE_H
#define UCFQUSIM_PRIORITYNODE_H


class PriorityNode {
private:
    int logicalQubit;
    int physicalQubit;
    int inDegree;
    int outDegree;
    int rank;
public:
    PriorityNode();
    PriorityNode(int logicalQubit, int physicalQubit, int inDegree, int outDegree);

    void incInDegree();
    void incOutDegree();

    int getLogicalQubit() const;

    int getPhysicalQubit() const;

    void setPhysicalQubit(int physicalQubit);

    int getInDegree() const;

    int getOutDegree() const;

    int getRank() const;

};


#endif //UCFQUSIM_PRIORITYNODE_H
