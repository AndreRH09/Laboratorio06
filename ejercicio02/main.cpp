#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

struct Record {
    int customer_id;
    std::string item_purchased;
    double review_rating;
    int manhattan_distance;

    Record(int id, std::string item, double rating) 
        : customer_id(id), item_purchased(item), review_rating(rating), manhattan_distance(0) {}

    bool operator<(const Record& other) const {
        return this->manhattan_distance < other.manhattan_distance;
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

        int distance = std::abs(target.customer_id - node->data.customer_id);
        node->data.manhattan_distance = distance;

        if (closest.size() < k) {
            closest.push(node->data);
        } else if (distance < closest.top().manhattan_distance) {
            closest.pop();
            closest.push(node->data);
        }

        if (target.customer_id < node->data.customer_id) {
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

        // Omitimos columnas intermedias
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

        // Omitimos las columnas restantes
        for (int i = 0; i < 9; ++i) getline(ss, temp, ',');  // Subscrición, método de pago, etc.

        records.emplace_back(customer_id, item_purchased, review_rating);
    }

    return records;
}

int main() {
    std::string filename = "../shopping_trends.csv";
    std::vector<Record> records = loadRecords(filename);

    if (records.empty()) {
        std::cout << "No records found!" << std::endl;
        return 0;
    }

    BST bst;
    for (const auto& record : records) {
        bst.insert(record);
    }

    Record target_record(6, "", 0); // Definir un registro de ejemplo como objetivo para las búsquedas cercanas
    int k = 6;
    std::vector<Record> closest_records = bst.getKClosest(k, target_record);

    std::cout << "Los " << k << " registros más cercanos son:\n";
    for (const auto& record : closest_records) {
        
        if(record.customer_id != target_record.customer_id){
            std::cout << "Customer ID: " << record.customer_id 
                  << ", Item Purchased: " << record.item_purchased 
                  << ", Review Rating: " << record.review_rating 
                  << ", Manhattan Distance: " << record.manhattan_distance << '\n';
        }
        else{
            std::cout << "El registro con Customer ID: " << record.customer_id 
                  << ", Item Purchased: " << record.item_purchased 
                  << ", Review Rating: " << record.review_rating << '\n'<<std::endl;
        }
        
    }

    return 0;
}
