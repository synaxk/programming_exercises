//
// Created by alex on 06.04.22.
//

#include <iostream>
#include "../headers/BinaryTree.h"
#include <climits>
BinaryTree::BinaryTree() = default;

BinaryTree::BinaryTree(std::list<int> &input) {
    nodes = 1;
    root = nullptr;
    min = INT_MAX;
    max = INT_MIN;
    avl = true;
    std::list<int>::iterator it;
    for (it = input.begin(); it != input.end(); ++it){
        tNode* newNode = createNode(*it);
        insertNode(newNode, this->root);
    }
}

tNode* BinaryTree::createNode(int value) {
    tNode* newNode = new tNode;

    newNode->key = value;
    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

BinaryTree::~BinaryTree() {

}

bool BinaryTree::insertNode(tNode *node, tNode* parent) {

    if (parent == nullptr){
        root = node;
        return true;
    }

    if (node->key > parent->key) {
        if (parent->right == nullptr) {
            parent->right = node;
            return true;
        } else {
            insertNode(node, parent->right);
        }
    } else if (node->key < parent->key){
        if (parent->left == nullptr) {
            parent->left = node;
            return true;
        } else {
            insertNode(node, parent->left);
        }
    } else {
        //duplicate key
        return false;
    }
    return false;
}

tNode *BinaryTree::getRoot() {
    return root;
}

void BinaryTree::getMinMaxKeyIt() {
    if (root == nullptr){
        return;
    }
    tNode* current = root;
    while (current->right != nullptr){
        current = current->right;
    }
    max = current->key;
    current = root;

    while (current->right != nullptr){
        current = current->left;
    }
    min = current->key;
}

void BinaryTree::getMinMaxKeyRec() {
    traverseMinMaxKey(this->max, this->min, root);
}

void BinaryTree::traverseMinMaxKey(int &maxKey, int &minKey, tNode *node) {
    if (node == nullptr){
        return;
    }
    if (node->key > maxKey){
        maxKey = node->key;
    }
    if (node->key < minKey){
        minKey = node->key;
    }
    traverseMinMaxKey(maxKey, minKey, node->left);
    traverseMinMaxKey(maxKey, minKey, node->right);
}

void BinaryTree::traverseDepth(int &depth, int &maxDepth, tNode *node) {
    if (node == nullptr) {
        return;
    }
    if (depth > maxDepth){
        maxDepth = depth;
    }
    traverseDepth(++depth, maxDepth, node->left);
    --depth;
    traverseDepth(++depth, maxDepth, node->right);
    --depth;
}

int BinaryTree::getMaxDepth(tNode* node) {
    int depth = 1, maxDepth = 0;
    traverseDepth(depth, maxDepth, node);
    return maxDepth;
}

void BinaryTree::checkAVL(tNode *node) {
    if (node == nullptr){
        return;
    }
    checkAVL(node->right);
    checkAVL(node->left);

    int bal = getBalance(node);

    std::cout << "bal(" << node->key << ") = " << bal;
    if (bal > 1 || bal < -1) {
        this->avl = false;
        std::cout << " (AVL violation!)";
    }
    std::cout << std::endl;
}

int BinaryTree::getBalance(tNode* node) {
    return abs(getMaxDepth(node->right) - getMaxDepth(node->left));
}

float BinaryTree::getAvgKey(tNode* node) {
    if (node->left == nullptr && node->right == nullptr) {
        return node->key;
    }
    float sum = 0;
    sum += node->key;
    if (node->left != nullptr) {
        this->nodes++;
        sum += getAvgKey(node->left);
    }
    if (node->right != nullptr) {
        this->nodes++;
        sum += getAvgKey(node->right);
    }

    if (node == this->root) {
        return sum / nodes;
    }
    return sum;
}

int BinaryTree::getMin() {
    return this->min;
}

int BinaryTree::getMax() {
    return this->max;
}


