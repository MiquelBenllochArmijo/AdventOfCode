#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <limits>
using namespace std;

// Función para fusionar dos subarreglos en orden
void merge(vector<int>& array, int ini, int fin) {
    int medio = ini + (fin - ini) / 2; // Punto medio del subarreglo
    int n1 = medio - ini + 1;         // Tamaño del subarreglo izquierdo
    int n2 = fin - medio;             // Tamaño del subarreglo derecho

    // Crear subarreglos temporales
    vector<int> left(n1), right(n2);

    // Copiar datos en los subarreglos
    for (int i = 0; i < n1; i++) {
        left[i] = array[ini + i];
    }
    for (int i = 0; i < n2; i++) {
        right[i] = array[medio + 1 + i];
    }

    // Fusionar los subarreglos ordenados
    int i = 0, j = 0, k = ini;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes del subarreglo izquierdo (si los hay)
    while (i < n1) {
        array[k] = left[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes del subarreglo derecho (si los hay)
    while (j < n2) {
        array[k] = right[j];
        j++;
        k++;
    }
}

// Función recursiva para implementar el algoritmo mergesort
void mergesort(vector<int>& array, int ini, int fin) {
    if (ini < fin) {
        int medio = ini + (fin - ini) / 2; // Calcular el punto medio
        mergesort(array, ini, medio);     // Ordenar la mitad izquierda
        mergesort(array, medio + 1, fin); // Ordenar la mitad derecha
        merge(array, ini, fin);           // Fusionar ambas mitades
    }
}

int main() {
    // Abrir el archivo de entrada
    ifstream file("day1_puzzle");
    if (!file) {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    // Vectores para almacenar las dos listas
    vector<int> left_side;
    vector<int> right_side;

    // Leer el archivo línea por línea
    string line;
    int number1, number2;
    while (getline(file, line)) {
        istringstream iss(line);
        if (iss >> number1 >> number2) {
            left_side.push_back(number1);
            right_side.push_back(number2);
        }
    }

    // Ordenar ambas listas usando mergesort
    mergesort(left_side, 0, left_side.size() - 1);
    mergesort(right_side, 0, right_side.size() - 1);

    // Calcular la distancia total entre ambas listas
    int distance = 0;
    for (size_t i = 0; i < left_side.size(); i++) {
        distance += abs(left_side[i] - right_side[i]);
    }

    // Mostrar el resultado
    cout << "Total distance: " << distance << std::endl;

    return 0;
}
