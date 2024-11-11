#include "LinkedList.h"

// Constructor
LinkedList::LinkedList() : head(nullptr) {}

// Destructor para liberar memoria
LinkedList::~LinkedList() {
    Node* current = head;
    while (current) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

// Inserta un valor al final de la lista
void LinkedList::insert(int value) {
    std::lock_guard<std::mutex> guard(listMutex); // Protección con mutex
    Node* newNode = new Node(value);
    if (!head) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    std::cout << "Insertando: " << value << std::endl;
}

// Elimina un valor de la lista
void LinkedList::remove(int value) {
    std::lock_guard<std::mutex> guard(listMutex); // Protección con mutex
    if (!head) {
        std::cout << "No se elimino: " << value << std::endl;
        return;
    }

    if (head->value == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        std::cout << "Eliminando: " << value << std::endl;
        return;
    }

    Node* current = head;
    while (current->next && current->next->value != value) {
        current = current->next;
    }

    if (current->next) {
        Node* temp = current->next;
        current->next = current->next->next;
        delete temp;
        std::cout << "Eliminando: " << value << std::endl;
    } else {
        std::cout << "No se elimino: " << value << std::endl;
    }

}

// Busca un valor en la lista
bool LinkedList::search(int value) {
    std::lock_guard<std::mutex> guard(listMutex); // Protección con mutex
    Node* current = head;
    while (current) {
        if (current->value == value) {
            std::cout << "Buscado: " << value << std::endl;
            return true;
        }
        current = current->next;
    }
    std::cout << "No encontrado: " << value << std::endl;
    return false;
}

// Modifica un valor en la lista
void LinkedList::modify(int oldValue, int newValue) {
    std::lock_guard<std::mutex> guard(listMutex); // Protección con mutex
    Node* current = head;
    while (current) {
        if (current->value == oldValue) {
            current->value = newValue;
            std::cout << "Modificando " << oldValue << " a " << newValue << std::endl;
            return;
        }
        current = current->next;
    }
    std::cout << "No se encontro: " << oldValue << std::endl;
}
