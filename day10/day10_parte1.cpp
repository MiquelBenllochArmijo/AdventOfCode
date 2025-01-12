#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

// Estructura para representar un nodo en el grafo.
struct Node {
    int x, y; // Coordenadas de la celda en el mapa.
    Node(int x, int y) : x(x), y(y) {}

    // Operador de comparación para que los nodos puedan ser comparados.
    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
};

// Hash personalizado para usar Node en contenedores como unordered_map o unordered_set.
struct NodeHash {
    std::size_t operator()(const Node& node) const {
        // Combina los hashes de las coordenadas x e y.
        return std::hash<int>()(node.x) ^ (std::hash<int>()(node.y) << 1);
    }
};

// Construye un grafo a partir del mapa.
std::unordered_map<Node, std::vector<Node>, NodeHash> build_graph(const std::vector<std::string>& map) {
    std::unordered_map<Node, std::vector<Node>, NodeHash> graph;
    int rows = map.size();
    int cols = map[0].size();

    // Direcciones para los vecinos (arriba, abajo, izquierda, derecha).
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Node current(i, j); // Nodo actual en el grafo.

            // Recorremos las direcciones posibles para encontrar vecinos.
            for (const auto& dir : directions) {
                int ni = i + dir.first;
                int nj = j + dir.second;

                // Verificamos que los vecinos estén dentro de los límites del mapa.
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                    // Solo agregamos una conexión si el vecino tiene un valor consecutivo.
                    if (map[ni][nj] == map[i][j] + 1) {
                        graph[current].emplace_back(ni, nj);
                    }
                }
            }
        }
    }

    return graph;
}

// Realiza una búsqueda en anchura (BFS) para contar caminos válidos desde un nodo inicial.
uint64_t bfs(const Node& start, const std::unordered_map<Node, std::vector<Node>, NodeHash>& graph, const std::vector<std::string>& map) {
    std::queue<Node> q; // Cola para la exploración BFS.
    std::unordered_set<Node, NodeHash> visited; // Conjunto de nodos visitados.
    uint64_t total = 0; // Contador de caminos válidos que llegan a '9'.

    q.push(start); // Inicia la exploración desde el nodo dado.
    visited.insert(start);

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        // Si alcanzamos un nodo con el valor '9', contamos este camino.
        if (map[current.x][current.y] == '9') {
            ++total;
            continue; // No exploramos más allá de un nodo '9'.
        }

        // Verificamos si el nodo actual tiene vecinos en el grafo.
        if (graph.find(current) == graph.end()) {
            continue;
        }

        // Explorar los vecinos del nodo actual.
        for (const auto& neighbor : graph.at(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return total;
}

int main() {
    // Abrir el archivo de entrada.
    std::ifstream file("day10_puzzle.txt");
    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo 'day25_puzzle.txt'." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> map;

    // Leer el archivo línea por línea para construir el mapa.
    while (std::getline(file, line)) {
        map.push_back(line);
    }

    // Validar que el mapa no esté vacío y tenga un formato consistente.
    if (map.empty()) {
        std::cerr << "Error: El mapa está vacío." << std::endl;
        return 1;
    }

    size_t expected_cols = map[0].size();
    for (const auto& row : map) {
        if (row.size() != expected_cols) {
            std::cerr << "Error: El mapa tiene filas de longitudes inconsistentes." << std::endl;
            return 1;
        }
    }

    // Construir el grafo a partir del mapa.
    auto graph = build_graph(map);

    uint64_t result = 0;

    // Buscar todas las celdas con '0' y realizar BFS desde esas posiciones.
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == '0') {
                Node start(i, j);
                result += bfs(start, graph, map);
            }
        }
    }

    // Imprimir el resultado.
    std::cout << "result: " << result << std::endl;

    return 0;
}
