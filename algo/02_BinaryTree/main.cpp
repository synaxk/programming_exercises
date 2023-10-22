#include <iostream>
#include "list"
#include "headers/BinaryTree.h"
#include "headers/ArgumentHandler.h"


std::list<int> readFile(std::string &fileName);
bool searchSubTree(tNode *mainRoot, tNode *searchRoot);

int main(int argc, char **argv) {
    ArgumentHandler inputHandler = ArgumentHandler(argc, argv);
    //inputHandler.baseFile = "nums.txt"; //TODO: remove - only for testing purpose
    std::list<int> input = readFile(inputHandler.baseFile);

    BinaryTree tree = BinaryTree(input);

    if (!inputHandler.subFile.empty()) {
        input.clear();
        input = readFile(inputHandler.subFile);
        BinaryTree subtree = BinaryTree(input);

        if (searchSubTree(tree.getRoot(), subtree.getRoot())) {
            std::cout << "Subtree found!" << std::endl;
        } else {
            std::cout << "Subtree not found!\n";
        }
        return 0;
    }

    tree.checkAVL(tree.getRoot());
    if (!tree.avl) {
        std::cout << "AVL: no\n";
    }
    tree.getMinMaxKeyIt();
    //tree.getMinMaxKeyRec();
    std::cout << "min: " << tree.getMin() << ", max: " << tree.getMax() << ", avg: " << tree.getAvgKey(tree.getRoot()) << std::endl;

    return 0;
}

std::list<int> readFile(std::string &fileName) {
    std::list<int> input;
    std::ifstream file(fileName);
    std::string value;
    for (int i = 0; getline(file, value); i++) {
        input.push_back(std::stoi(value));
    }
    return input;
}

bool searchSubTree(tNode *mainRoot, tNode *searchRoot) { //
    if (mainRoot == nullptr) {
        return false;
    }
    /// current node of searchRoot found
    if (searchRoot->key == mainRoot->key) {
        std::cout << searchRoot->key << " found!" << std::endl;
        /// check left and right for descendants
        if (searchRoot->left != nullptr) {
            return searchSubTree(mainRoot, searchRoot->left);
        }
        if (searchRoot->right != nullptr) {
            return searchSubTree(mainRoot, searchRoot->right);
        }
        return true;
    }

    /// search left or right
    if (searchRoot->key > mainRoot->key) {
        if (mainRoot->right == nullptr) {
            return false;
        }
        /// only print the key if the recursive call returns true
        /// => path is not printed if key is not found
        if (searchSubTree(mainRoot->right, searchRoot)) {
            std::cout << "Key: " << mainRoot->key << std::endl;
            return true;
        }
    } else {
        if (mainRoot->left == nullptr) {
            return false;
        }
        if (searchSubTree(mainRoot->left, searchRoot)) {
            std::cout << "Key: " << mainRoot->key << std::endl;
            return true;
        }
    }
    return false;
}