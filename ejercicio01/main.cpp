#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include "LinkedList.h"

// Función que genera un número aleatorio
int GetRandom(int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, max);
    return dis(gen);
}

// Hilo 1: Elimina valores aleatorios
void thread1(LinkedList& list) {
    for (int i = 0; i < 10; ++i) {
        int value = GetRandom(100);
        list.remove(value);
    }
    std::cout << ""<< std::endl;

}

// Hilo 2: Inserta valores aleatorios
void thread2(LinkedList& list) {
    for (int i = 0; i < 10; ++i) {
        int value = GetRandom(100);
        list.insert(value);
    }
    std::cout << ""<< std::endl;

}

// Hilo 3: Busca valores aleatorios
void thread3(LinkedList& list) {
    for (int i = 0; i < 10; ++i) {
        int value = GetRandom(100);
        list.search(value);
    }
    std::cout << ""<< std::endl;

}

// Hilo 4: Modifica valores aleatorios
void thread4(LinkedList& list) {
    for (int i = 0; i < 10; ++i) {
        int oldValue = GetRandom(100);
        int newValue = GetRandom(100);
        list.modify(oldValue, newValue);
    }
    std::cout << ""<< std::endl;

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LinkedList list;

    // Crear los hilos
    std::thread t1(thread1, std::ref(list));
    std::thread t2(thread2, std::ref(list));
    std::thread t3(thread3, std::ref(list));
    std::thread t4(thread4, std::ref(list));

    // Esperar a que terminen los hilos
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return a.exec();
}
