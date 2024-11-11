#include "BinaryTree.h"

BinaryTree::BinaryTree() : root(nullptr) {}

void BinaryTree::insert(int id, const QString& item, double rating) {
    root = insertRec(root, id, item, rating);
}

Node* BinaryTree::getRoot() {
    return root;
}

Node* BinaryTree::insertRec(Node* node, int id, const QString& item, double rating) {
    if (node == nullptr) {
        return new Node(id, item, rating);
    }
    if (id < node->customerId) {
        node->left = insertRec(node->left, id, item, rating);
    } else {
        node->right = insertRec(node->right, id, item, rating);
    }
    return node;
}
