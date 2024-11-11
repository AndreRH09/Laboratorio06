#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Node.h"

class BinaryTree {
public:
    BinaryTree();
    void insert(int id, const QString& item, double rating);
    Node* getRoot();

private:
    Node* root;
    Node* insertRec(Node* node, int id, const QString& item, double rating);
};

#endif // BINARYTREE_H
