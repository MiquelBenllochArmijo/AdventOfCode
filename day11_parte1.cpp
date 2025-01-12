#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory> 
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// Constante que define el nivel máximo de profundidad para el conteo
constexpr int MAX_LEVEL = 25;

// Estructura que define un nodo en el árbol
struct Node {
    long long engraving; // Valor del nodo
    Node* left = nullptr; // Hijo izquierdo
    Node* right = nullptr; // Hijo derecho
};

// Cola global para gestionar los nodos en proceso
std::queue<std::pair<Node*, int>> node_queue;

// Función para generar nodos del árbol según reglas específicas
void generate_nodes(std::vector<std::unique_ptr<Node>>& nodes, std::unordered_map<long long, Node*>& seen_nodes) {
    while (!node_queue.empty()) {
        auto [node, level] = node_queue.front();
        node_queue.pop();

        // Limitar la profundidad para evitar procesamiento innecesario
        if (level >= 75) continue;

        // Calcular el número de dígitos del valor actual
        int num_digits = 0;
        {
            auto engraving = node->engraving;
            while (engraving > 0) {
                num_digits++;
                engraving /= 10;
            }
        }

        if (node->engraving == 0) {
            // Caso: valor 0 -> Crear nodo con valor 1
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
        } else if (num_digits % 2 == 0) {
            // Caso: número de dígitos par -> Dividir en dos mitades
            long long divisor = static_cast<long long>(std::pow(10, num_digits / 2));
            
            // Crear nodo izquierdo con la parte superior de los dígitos
            auto left_node = std::make_unique<Node>();
            left_node->engraving = node->engraving / divisor;
            if (seen_nodes.find(left_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(left_node));
                node->left = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->left = seen_nodes[left_node->engraving];
            }

            // Crear nodo derecho con la parte inferior de los dígitos
            auto right_node = std::make_unique<Node>();
            right_node->engraving = node->engraving % divisor;
            if (seen_nodes.find(right_node->engraving) == seen_nodes.end()) {
                nodes.push_back(std::move(right_node));
                node->right = nodes.back().get();
                seen_nodes[nodes.back()->engraving] = nodes.back().get();
                node_queue.push({nodes.back().get(), level + 1});
            } else {
                node->right = seen_nodes[right_node->engraving];
            }
        } else {
            // Caso: número de dígitos impar -> Multiplicar por 2024
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

// Almacenamiento en memoria para resultados intermedios (Memoization)
std::unordered_map<long long, std::unordered_map<int, long long>> memo;

// Función para contar nodos hasta el nivel especificado
long long count_nodes(Node* node, int level) {
    if (level == MAX_LEVEL) return 1; // Llegamos al nivel máximo
    if (memo[node->engraving][level] != 0) return memo[node->engraving][level]; // Usar resultados memorizados

    long long total = 0;
    if (node->left != nullptr) total += count_nodes(node->left, level + 1);
    if (node->right != nullptr) total += count_nodes(node->right, level + 1);

    memo[node->engraving][level] = total; // Memorizar resultado
    return total;
}

int main(int argc, char* argv[]) {
    // Leer archivo de entrada
    std::string input_file = "day11_puzzle.txt";
    if (argc > 1) {
        input_file = argv[1];
    }

    std::ifstream file(input_file);
    std::string line;

    while (std::getline(file, line)) {
        // Parsear los valores de entrada
        std::vector<std::unique_ptr<Node>> nodes;
        size_t current = 0;
        size_t next_space = line.find(' ');

        while (next_space != std::string::npos) {
            auto new_node = std::make_unique<Node>();
            new_node->engraving = std::stoll(line.substr(current, next_space - current));
            nodes.push_back(std::move(new_node));
            current = next_space + 1;
            next_space = line.find(' ', current);
        }

        auto new_node = std::make_unique<Node>();
        new_node->engraving = std::stoll(line.substr(current));
        nodes.push_back(std::move(new_node));

        // Calcular el resultado
        long long answer = 0;
        for (auto& node : nodes) {
            std::vector<std::unique_ptr<Node>> temp_nodes;
            temp_nodes.push_back(std::move(node));

            std::unordered_map<long long, Node*> seen_nodes;
            seen_nodes[temp_nodes.back()->engraving] = temp_nodes.back().get();

            node_queue.push({temp_nodes.back().get(), 0});
            generate_nodes(temp_nodes, seen_nodes);

            answer += count_nodes(temp_nodes[0].get(), 0);
        }

        std::cout << answer << '\n';
    }

    return 0;
}
