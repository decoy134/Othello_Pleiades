#ifndef __DECTREE_H__
#define __DECTREE_H__

#include "common.hpp"
#include "player.hpp"

using namespace std;

// Rudimentary decision tree, still under construction!!!

// 
struct CompareDecisions
{
    bool operator() (Decision x, Decision y)
    {
        if(x.score < y.score)
            return true;
        return false;
    }
}

struct Decision
{
    int x, y; // x and y coordinates of the move stored
    double score; // score of this move and its depth

    Decision parent; // parent node to retrieve best path

    ~Decision()
    {
    }
};

Node::Node(int x_coor, y_coor) : x(x_coor), y(y_coor), score(0) {}

class DecisionTree {

private:
    Decision root;

public:
    DecisionTree(int x, int y);
    ~DecisionTree();
    void addNode(int x, int y);
};

DecisionTree::DecisionTree(int x, int y) : root(x, y) {}

DecisionTree::~DecisionTree() {}

#endif
