#ifndef NODE_H
#define NODE_H

#include <QString>

class Node {
public:
    int customerId;
    QString itemPurchased;
    double reviewRating;
    Node* left;
    Node* right;

    Node(int id, const QString& item, double rating);
};

#endif // NODE_H
