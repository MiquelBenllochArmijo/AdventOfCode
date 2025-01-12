#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory> 
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int MAX_LEVEL = 75;

// Estructura que representa un nodo en el árbol
struct Node {
    long long value; // "Engraving" en el código original
    Node* left;
    Node* right; 
};

// Cola para gestionar los nodos pendientes de procesar
std::queue<std::pair<Node*, int>> node_queue;

// Función para procesar los nodos y generar nuevos nodos según las reglas
void processNodes(std::vector<std::unique_ptr<Node>>& nodes, std::unordered_map<long long, Node*>& seen_nodes) {
    while (!node_queue.empty()) {
        auto [current_node, level] = node_queue.front();
        node_queue.pop();

        // Si se alcanza el nivel máximo, no procesar más
        if (level >= MAX_LEVEL) continue;

        // Contar la cantidad de dígitos en el valor del nodo
        int num_digits = 0;
        long long temp_value = current_node->value;
        while (temp_value > 0) {
            num_digits++;
            temp_value /= 10;
        }

        // Si el valor es 0, crear un nuevo nodo con valor 1
        if (current_node->value == 0) {
            auto new_node = std::make_unique<Node>();
            new_node->value = 1;

            if (seen_nodes.find(new_node->value) == seen_nodes.end()) {
                nodes.push_back(std::move(new_node));
                current_node->left = nodes.back().get();
                seen_nodes[nodes.back()->value] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                current_node->left = seen_nodes[new_node->value];
            }
        } 
        // Si el número de dígitos es par, dividir el valor en dos partes
        else if (num_digits % 2 == 0) {
            // Crear nodo para la parte izquierda (dígitos más significativos)
            auto left_node = std::make_unique<Node>();
            left_node->value = current_node->value / static_cast<long long>(std::pow(10, num_digits / 2));

            if (seen_nodes.find(left_node->value) == seen_nodes.end()) {
                nodes.push_back(std::move(left_node));
                current_node->left = nodes.back().get();
                seen_nodes[nodes.back()->value] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                current_node->left = seen_nodes[left_node->value];
            }

            // Crear nodo para la parte derecha (dígitos menos significativos)
            auto right_node = std::make_unique<Node>();
            right_node->value = current_node->value % static_cast<long long>(std::pow(10, num_digits / 2));

            if (seen_nodes.find(right_node->value) == seen_nodes.end()) {
                nodes.push_back(std::move(right_node));
                current_node->right = nodes.back().get();
                seen_nodes[nodes.back()->value] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                current_node->right = seen_nodes[right_node->value];
            }
        } 
        // Si el número de dígitos es impar, multiplicar por 2024
        else {
            auto new_node = std::make_unique<Node>();
            new_node->value = current_node->value * 2024;

            if (seen_nodes.find(new_node->value) == seen_nodes.end()) {
                nodes.push_back(std::move(new_node));
                current_node->left = nodes.back().get();
                seen_nodes[nodes.back()->value] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                current_node->left = seen_nodes[new_node->value];
            }
        }
    }
}

// Memoria para memoización de los cálculos
std::unordered_map<long long, std::unordered_map<long long, long long>> memoization;

// Función recursiva para contar los caminos en el árbol
long long countPaths(Node* node, int level) {
    if (level == MAX_LEVEL) return 1; // Base: se alcanzó el nivel máximo

    // Si ya está memorizado, devolver el resultado almacenado
    if (memoization[node->value].count(level)) 
        return memoization[node->value][level];

    if (!node->left && !node->right) return 1; // Nodo hoja

    long long path_count = 0;
    if (node->left) path_count += countPaths(node->left, level + 1);
    if (node->right) path_count += countPaths(node->right, level + 1);

    memoization[node->value][level] = path_count;
    return path_count;
}

int main(int argc, char* argv[]) {
    // Leer el archivo de entrada
    std::string input_file = "day11_puzzle.txt";
    if (argc > 1) {
        input_file = argv[1];
    }

    std::ifstream file(input_file);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::unique_ptr<Node>> initial_nodes;
        size_t current_pos = 0;
        size_t next_space = line.find(' ');

        // Crear nodos iniciales a partir de los números en la línea
        while (next_space != std::string::npos) {
            auto new_node = std::make_unique<Node>();
            new_node->value = std::stoll(line.substr(current_pos, next_space - current_pos));
            initial_nodes.push_back(std::move(new_node));
            current_pos = next_space + 1;
            next_space = line.find(' ', current_pos);
        }

        auto last_node = std::make_unique<Node>();
        last_node->value = std::stoll(line.substr(current_pos));
        initial_nodes.push_back(std::move(last_node));

        long long total_paths = 0;

        // Procesar cada nodo inicial
        for (auto& node : initial_nodes) {
            std::vector<std::unique_ptr<Node>> tree_nodes;
            tree_nodes.push_back(std::move(node));

            std::unordered_map<long long, Node*> seen_nodes;
            node_queue = {};
            node_queue.push({tree_nodes.back().get(), 0});
            seen_nodes[tree_nodes.back()->value] = tree_nodes.back().get();

            processNodes(tree_nodes, seen_nodes);
            total_paths += countPaths(tree_nodes[0].get(), 0);
        }

        std::cout << total_paths << '\n';
    }

    return 0;
}
