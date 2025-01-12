#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

// Función auxiliar para determinar si una transición es válida
bool is_valid_transition(int prev, int curr, bool increasing) {
    int diff = curr - prev;
    if (increasing) {
        return diff >= 1 && diff <= 3;
    } else {
        return diff >= -3 && diff <= -1;
    }
}

// Función para verificar si un reporte es seguro usando programación dinámica
bool is_safe_dp(const std::vector<int>& report) {
    int n = report.size();
    if (n < 2) return false; // Un reporte con menos de 2 elementos no es seguro

    // Matriz dp: dp[i][0] = es seguro como decreciente hasta i
    //            dp[i][1] = es seguro como creciente hasta i
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(2, false));

    // Inicialización: el primer elemento es seguro por sí mismo
    dp[0][0] = dp[0][1] = true;

    // Llenado de la tabla
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j][0] && is_valid_transition(report[j], report[i], false)) {
                dp[i][0] = true;
            }
            if (dp[j][1] && is_valid_transition(report[j], report[i], true)) {
                dp[i][1] = true;
            }
        }
    }

    // Un reporte es seguro si es creciente o decreciente hasta el último elemento
    return dp[n - 1][0] || dp[n - 1][1];
}

// Función principal para verificar la seguridad de múltiples reportes
int check_safeness_of_reports(const std::vector<std::vector<int>>& reports) {
    int safe_reports = 0;

    for (size_t i = 0; i < reports.size(); i++) {
        if (is_safe_dp(reports[i])) {
            safe_reports++;
            std::cout << (i + 1) << ": Safe\n";
        } else {
            std::cout << (i + 1) << ": Unsafe\n";
        }
    }

    return safe_reports;
}

int main() {
    std::ifstream file("day2_puzzle.txt");
    if (!file) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> reports;
    std::string line;
    int number;

    while (std::getline(file, line)) {
        reports.push_back({});
        std::istringstream iss(line);
        while (iss >> number) {
            reports.back().push_back(number);
        }
    }

    int reports_safe = check_safeness_of_reports(reports);
    std::cout << "Total safe reports: " << reports_safe << std::endl;

    return 0;
}
