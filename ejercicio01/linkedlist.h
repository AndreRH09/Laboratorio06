#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <mutex>
#include <iostream>

// Nodo de la lista enlazada
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

// Clase LinkedList
class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void insert(int value);
    void remove(int value);
    bool search(int value);
    void modify(int oldValue, int newValue);

private:
    Node* head;
    std::mutex listMutex;  // Mutex para controlar acceso a la lista
};

#endif // LINKEDLIST_H
