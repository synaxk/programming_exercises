//
// Created by alex on 06.04.22.
//

#ifndef UE_2_BINARYTREE_H
#define UE_2_BINARYTREE_H
#include "list"

typedef struct tNode {
    int key;
    struct tNode *left;
    struct tNode *right;
} tNode;

class BinaryTree {

public:
    BinaryTree();
    BinaryTree(std::list<int> &input);
    ~BinaryTree();
    static tNode* createNode(int value);
    bool insertNode(tNode* node, tNode* parent);
    void traverseMinMaxKey(int &maxKey, int &minKey, tNode* node);
    void traverseDepth(int &depth, int &maxDepth, tNode* node);
    int getMaxDepth(tNode* node);
    void getMinMaxKeyRec();
    tNode* getRoot();
    void checkAVL(tNode* node);
    int getBalance(tNode* node);
    float getAvgKey(tNode* node);
    int getMin();
    int getMax();
    void getMinMaxKeyIt();
    bool avl;

private:
    tNode* root;
    int nodes;
    int min;
    int max;
};


#endif //UE_2_BINARYTREE_H
