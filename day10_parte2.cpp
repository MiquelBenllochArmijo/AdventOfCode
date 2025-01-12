#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>

// Estructura para representar un nodo en el grafo.
struct Node {
    int x, y;
    char value;

    Node(int x, int y, char value) : x(x), y(y), value(value) {}

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
};

// Hash personalizado para usar Node en unordered_map o unordered_set.
struct NodeHash {
    std::size_t operator()(const Node& node) const {
        return std::hash<int>()(node.x) ^ (std::hash<int>()(node.y) << 1);
    }
};

// Función para construir nodos con adyacencias en el grafo.
std::unordered_map<Node, std::vector<Node>, NodeHash> build_graph(const std::vector<std::string>& map) {
    std::unordered_map<Node, std::vector<Node>, NodeHash> graph;
    int rows = map.size();
    int cols = map[0].size();

    // Direcciones posibles (arriba, abajo, izquierda, derecha).
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Node current(i, j, map[i][j]);
            for (const auto& dir : directions) {
                int ni = i + dir.first;
                int nj = j + dir.second;

                // Verificamos límites y que el valor sea consecutivo.
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                    if (map[ni][nj] == map[i][j] + 1) {
                        graph[current].emplace_back(ni, nj, map[ni][nj]);
                    }
                }
            }
        }
    }

    return graph;
}

// Función recursiva para buscar caminos a '9' desde un nodo.
uint64_t count_paths_to_nine(const Node& start, const std::unordered_map<Node, std::vector<Node>, NodeHash>& graph, std::unordered_set<Node, NodeHash>& visited) {
    // Si el nodo actual tiene el valor '9', contamos este camino.
    if (start.value == '9') {
        return 1;
    }

    // Marcamos el nodo como visitado.
    visited.insert(start);

    uint64_t total_paths = 0;

    // Verificamos si el nodo tiene vecinos en el grafo.
    auto it = graph.find(start); // Usamos find() para evitar errores si el nodo no está.
    if (it != graph.end()) {
        for (const auto& neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                total_paths += count_paths_to_nine(neighbor, graph, visited);
            }
        }
    }

    // Desmarcamos el nodo al retroceder en la recursión.
    visited.erase(start);
    return total_paths;
}

int main() {
    std::ifstream file("day10_puzzle.txt");
    if (!file) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> map;

    // Leer el mapa desde el archivo.
    while (std::getline(file, line)) {
        map.push_back(line);
    }

    // Validar que el mapa no esté vacío.
    if (map.empty() || map[0].empty()) {
        std::cerr << "Error: El mapa está vacío o no es válido." << std::endl;
        return 1;
    }

    // Construir el grafo a partir del mapa.
    auto graph = build_graph(map);

    uint64_t result = 0;

    // Buscar caminos desde cada nodo '0'.
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == '0') {
                Node start(i, j, '0');
                std::unordered_set<Node, NodeHash> visited;
                result += count_paths_to_nine(start, graph, visited);
            }
        }
    }

    std::cout << "result: " << result << std::endl;

    return 0;
}
