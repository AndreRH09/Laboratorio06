#include "Node.h"

Node::Node(int id, const QString& item, double rating)
    : customerId(id), itemPurchased(item), reviewRating(rating), left(nullptr), right(nullptr) {}
