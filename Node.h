#ifndef CS236_LAB5_T2K_DATALOG_NODE_H
#define CS236_LAB5_T2K_DATALOG_NODE_H

#include <set>
#include <sstream>

struct Node
{
public:
    Node() : hasBeenVisited(false), postOrderId(-1){};

    std::set<int> adjacentNodeIds;
    bool hasBeenVisited;
    int postOrderId;

    void addEdge(int receivingNodeId)
    {
        adjacentNodeIds.insert(receivingNodeId);
    }

    std::string toString() const
    {
        std::stringstream out;
        for (std::set<int>::iterator i = adjacentNodeIds.begin(); i != adjacentNodeIds.end(); i++)
        {
            if (i == adjacentNodeIds.begin())
            {
                out << "R" << *i;
            }
            else
            {
                out << ",R" << *i;
            }
        }
        return out.str();
    }
};

#endif