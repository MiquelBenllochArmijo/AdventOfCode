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
    long long engraving; // Cambiado de "value" a "engraving"
    Node* left;
    Node* right; 
};

// Cola para gestionar los nodos pendientes de procesar
std::queue<std::pair<Node*, int>> node_queue;

// Función para procesar los nodos y generar nuevos nodos según las reglas
void generate_nodes(std::vector<std::unique_ptr<Node>>& nodes, std::unordered_map<long long, Node*>& seen_nodes) {
    while (!node_queue.empty()) {
        auto [node, level] = node_queue.front();
        node_queue.pop();

        // Si se alcanza el nivel máximo, no procesar más
        if (level >= MAX_LEVEL) continue;

        // Contar la cantidad de dígitos en el valor del nodo
        int num_digits = 0;
        long long engraving = node->engraving;
        while (engraving > 0) {
            num_digits++;
            engraving /= 10;
        }

        // Si el valor es 0, crear un nuevo nodo con valor 1
        if (node->engraving == 0) {
            auto new_node = std::make_unique<Node>();
            new_node->engraving = 1;

            if (seen_nodes.find(new_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(new_node));
                node->left = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->left = seen_nodes[new_node->engraving];
            }
        } 
        // Si el número de dígitos es par, dividir el valor en dos partes
        else if (num_digits % 2 == 0) {
            // Crear nodo para la parte izquierda (dígitos más significativos)
            auto left_node = std::make_unique<Node>();
            left_node->engraving = node->engraving / static_cast<long long>(std::pow(10, num_digits / 2));

            if (seen_nodes.find(left_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(left_node));
                node->left = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->left = seen_nodes[left_node->engraving];
            }

            // Crear nodo para la parte derecha (dígitos menos significativos)
            auto right_node = std::make_unique<Node>();
            right_node->engraving = node->engraving % static_cast<long long>(std::pow(10, num_digits / 2));

            if (seen_nodes.find(right_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(right_node));
                node->right = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->right = seen_nodes[right_node->engraving];
            }
        } 
        // Si el número de dígitos es impar, multiplicar por 2024
        else {
            auto new_node = std::make_unique<Node>();
            new_node->engraving = node->engraving * 2024;

            if (seen_nodes.find(new_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(new_node));
                node->left = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->left = seen_nodes[new_node->engraving];
            }
        }
    }
}

// Memoria para memoización de los cálculos
std::unordered_map<long long, std::unordered_map<int, long long>> memo;

// Función recursiva para contar los caminos en el árbol
long long count_nodes(Node* node, int level) {
    if (level == MAX_LEVEL) return 1; // Base: se alcanzó el nivel máximo

    // Si ya está memorizado, devolver el resultado almacenado
    if (memo[node->engraving][level] != 0) 
        return memo[node->engraving][level];

    long long total = 0;
    if (node->left) total += count_nodes(node->left, level + 1);
    if (node->right) total += count_nodes(node->right, level + 1);

    memo[node->engraving][level] = total;
    return total;
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
        std::vector<std::unique_ptr<Node>> nodes;
        size_t current_pos = 0;
        size_t next_space = line.find(' ');

        // Crear nodos iniciales a partir de los números en la línea
        while (next_space != std::string::npos) {
            auto new_node = std::make_unique<Node>();
            new_node->engraving = std::stoll(line.substr(current_pos, next_space - current_pos));
            nodes.push_back(std::move(new_node));
            current_pos = next_space + 1;
            next_space = line.find(' ', current_pos);
        }

        auto last_node = std::make_unique<Node>();
        last_node->engraving = std::stoll(line.substr(current_pos));
        nodes.push_back(std::move(last_node));

        long long total_nodes = 0;

        // Procesar cada nodo inicial
        for (auto& node : nodes) {
            std::vector<std::unique_ptr<Node>> tree_nodes;
            tree_nodes.push_back(std::move(node));

            std::unordered_map<long long, Node*> seen_nodes;
            node_queue = {};
            node_queue.push({tree_nodes.back().get(), 0});
            seen_nodes[tree_nodes.back()->engraving] = tree_nodes.back().get();

            generate_nodes(tree_nodes, seen_nodes);
            total_nodes += count_nodes(tree_nodes[0].get(), 0);
        }

        std::cout << total_nodes << '\n';
    }

    return 0;
}
