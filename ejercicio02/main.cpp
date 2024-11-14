#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>

std::mutex mtx;

struct Record {
    int customer_id;
    std::string item_purchased;
    double review_rating;
    double manhattan_distance;

    Record(int id, std::string item, double rating)
        : customer_id(id), item_purchased(item), review_rating(rating), manhattan_distance(0.0) {}

    bool operator<(const Record& other) const {
        return this->manhattan_distance > other.manhattan_distance;
    }
};

class BST {
public:
    struct Node {
        Record data;
        Node* left;
        Node* right;
        Node(Record rec) : data(rec), left(nullptr), right(nullptr) {}
    };

    BST() : root(nullptr) {}

    void insert(Record record) {
        std::lock_guard<std::mutex> lock(mtx);  // Bloqueo para proteger el acceso al árbol
        root = insertRec(root, record);
    }

    std::vector<Record> getKClosest(int k, const Record& target) {
        std::priority_queue<Record> closest;
        findKClosest(root, target, k, closest);

        std::vector<Record> result;
        while (!closest.empty()) {
            result.push_back(closest.top());
            closest.pop();
        }
        std::reverse(result.begin(), result.end()); // Ordenar de más cercano a menos
        return result;
    }

private:
    Node* root;

    Node* insertRec(Node* node, Record record) {
        if (node == nullptr) {
            return new Node(record);
        }
        if (record.manhattan_distance < node->data.manhattan_distance) {
            node->left = insertRec(node->left, record);
        } else {
            node->right = insertRec(node->right, record);
        }
        return node;
    }

    void findKClosest(Node* node, const Record& target, int k, std::priority_queue<Record>& closest) {
        if (node == nullptr) return;

        double review_distance = std::abs(target.review_rating - node->data.review_rating);
        double item_distance = (target.item_purchased == node->data.item_purchased) ? 0.0 : 1.0;
        double total_distance = review_distance + item_distance;

        node->data.manhattan_distance = total_distance;

        if (closest.size() < k) {
            closest.push(node->data);
        } else if (total_distance < closest.top().manhattan_distance) {
            closest.pop();
            closest.push(node->data);
        }

        if (target.review_rating < node->data.review_rating) {
            findKClosest(node->left, target, k, closest);
        } else {
            findKClosest(node->right, target, k, closest);
        }
    }
};

std::vector<Record> loadRecords(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record> records;

    std::string line;
    getline(file, line); // Saltar la primera línea del encabezado

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string temp;
        int customer_id;
        std::string item_purchased;
        double review_rating;

        getline(ss, temp, ',');
        customer_id = std::stoi(temp);

        getline(ss, temp, ',');  // Edad
        getline(ss, temp, ',');  // Género
        getline(ss, item_purchased, ',');  // Artículo comprado
        getline(ss, temp, ',');  // Categoría
        getline(ss, temp, ',');  // Monto de la compra
        getline(ss, temp, ',');  // Ubicación
        getline(ss, temp, ',');  // Talla
        getline(ss, temp, ',');  // Color
        getline(ss, temp, ',');  // Temporada
        getline(ss, temp, ',');  // Calificación de la reseña
        review_rating = std::stod(temp);

        for (int i = 0; i < 9; ++i) getline(ss, temp, ',');

        records.emplace_back(customer_id, item_purchased, review_rating);
    }

    return records;
}

void insertRecordsToBST(BST& bst, const std::vector<Record>& records) {
    for (const auto& record : records) {
        bst.insert(record);
    }
}

int main() {
    std::string filename = "C:/Users/USER/OneDrive/Documentos/Labo06_02/shopping_trends.csv";
    std::vector<Record> records = loadRecords(filename);

    if (records.empty()) {
        std::cout << "No records found!" << std::endl;
        return 0;
    }

    BST bst;

    // Crear un hilo para insertar registros en el BST
    std::thread insertThread(insertRecordsToBST, std::ref(bst), std::cref(records));

    // Esperar a que termine el hilo de inserción
    insertThread.join();

    Record target_record(6, "target_item", 4.5); // Cambia "target_item" por el item deseado
    int k = 5; // Obtener los 5 valores más bajos de distancia

    // Crear un hilo para obtener los k registros más cercanos
    std::vector<Record> closest_records;
    std::thread searchThread([&]() {
        closest_records = bst.getKClosest(k, target_record);
    });

    // Esperar a que termine el hilo de búsqueda
    searchThread.join();

    // Mostrar los registros más cercanos
    std::cout << "Los " << k << " registros más cercanos son:\n";
    for (const auto& record : closest_records) {
        std::cout << "Item Purchased: " << record.item_purchased
                  << ", Review Rating: " << record.review_rating
                  << ", Manhattan Distance: " << record.manhattan_distance << '\n';
    }

    return 0;
}
