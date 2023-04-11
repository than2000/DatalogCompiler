#ifndef CS236_PROJECT5_T2K_DATALOG_DEPTH_FIRST_SEARCHER_H
#define CS236_PROJECT5_T2K_DATALOG_DEPTH_FIRST_SEARCHER_H

#include <vector>
#include "Graph.h"

// TODO. Make lists into refs

class DepthFirstSearcher
{
public:
    static std::vector<int> generatePostorder(Graph &graph);
    static std::vector<std::set<int>> generateForestWithRootPriorityList(Graph &graph, std::vector<int> rootPriorityList);
    virtual ~DepthFirstSearcher(){};

protected:
    DepthFirstSearcher(Graph *_graphPtr)
        : graphPtr(_graphPtr), nextRootNodeId(0), currentDfsTreeIndex(0){};

    Graph *graphPtr;
    int nextRootNodeId;
    int currentDfsTreeIndex;
    std::vector<int> visitSequence;
    std::vector<int> postOrderSequence;
    std::vector<std::set<int>> dfsForest;

    void search();
    void resetAllNodes();

private:
    void searchTree(Node &root);
    void recurseDepthFirstSearch(Node &node);
    void recordVisit(Node &node);
    void visitNeighborsOf(Node &node);
    virtual bool isSearchComplete();
    Node &getNextRoot();
    virtual void updateNextRootNode();
};

class PrioritizedDepthFirstSearcher : DepthFirstSearcher
{
public:
    static std::vector<std::set<int>> generateForestWithRootPriorityList(Graph &graph, std::vector<int> rootPriorityList);
    ~PrioritizedDepthFirstSearcher(){};

private:
    PrioritizedDepthFirstSearcher(
        Graph *_graphPtr,
        std::vector<int> _rootPriorityList)
        : DepthFirstSearcher(_graphPtr),
          rootPriorityList(_rootPriorityList){};

    std::vector<int> rootPriorityList;

    bool isSearchComplete();
    void updateNextRootNode();

    unsigned priorityListSize();
};

#endif
