#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Variables globales para el algoritmo
int filas, columnas, largo_c;
int max_listones = 0;
vector<vector<int>> tablero;
vector<vector<int>> mejor_tablero;

/* * ARREGLOS DE DIRECCIÓN:
 * Según las reglas, los movimientos se prueban en forma horaria empezando de las 6 horas.
 */

// Direcciones para formar el listón de tamaño c (Ortogonales: Abajo, Izquierda, Arriba, Derecha)
int dir_f[] = {1, 0, -1, 0};
int dir_c[] = {0, -1, 0, 1};

// Direcciones para buscar la celda adyacente donde iniciará el próximo listón (8 direcciones)
// Orden: Abajo(6h), Abajo-Izq(7.5h), Izq(9h), Arriba-Izq(10.5h), Arriba(12h), Arriba-Der(1.5h), Der(3h), Abajo-Der(4.5h)
int adj_f[] = {1, 1, 0, -1, -1, -1, 0, 1};
int adj_c[] = {0, -1, -1, -1, 0, 1, 1, 1};


/*
 * FORMA DE SOLUCIÓN (BACKTRACKING):
 * La función intenta colocar un listón de longitud 'c' empezando en (fila, col) probando las
 * 4 direcciones ortogonales. Si logra colocarlo, actualiza el máximo global y guarda el tablero.
 * Luego, a partir de la celda final de este listón recién colocado, busca en sus 8 celdas adyacentes
 * una posición vacía para iniciar el siguiente listón (llamada recursiva). Finalmente, retira el
 * listón (backtracking) para explorar otros caminos.
 */
void resolver(int fila, int col, int id_liston) {
    // Probar las 4 posibles direcciones para trazar el listón
    for (int i = 0; i < 4; i++) {
        int df = dir_f[i];
        int dc = dir_c[i];

        bool valido = true;

        // Verificar si las 'c' celdas están dentro del tablero y están vacías
        for (int k = 0; k < largo_c; k++) {
            int nf = fila + k * df;
            int nc = col + k * dc;

            if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas || tablero[nf][nc] != 0) {
                valido = false;
                break;
            }
        }

        if (valido) {
            // 1. Marcar el tablero con el id del listón (Hacer)
            for (int k = 0; k < largo_c; k++) {
                tablero[fila + k * df][col + k * dc] = id_liston;
            }

            // Si encontramos una cantidad mayor de listones, guardamos este estado
            if (id_liston > max_listones) {
                max_listones = id_liston;
                mejor_tablero = tablero;
            }

            // 2. Obtener la última posición del listón actual
            int fin_f = fila + (largo_c - 1) * df;
            int fin_c = col + (largo_c - 1) * dc;

            // 3. Buscar posiciones adyacentes para el SIGUIENTE listón
            for (int j = 0; j < 8; j++) {
                int nf = fin_f + adj_f[j];
                int nc = fin_c + adj_c[j];

                // Si la celda adyacente es válida y está vacía, iniciamos el siguiente ahí
                if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && tablero[nf][nc] == 0) {
                    resolver(nf, nc, id_liston + 1);
                }
            }

            // 4. Desmarcar el tablero (Deshacer / Backtrack)
            for (int k = 0; k < largo_c; k++) {
                tablero[fila + k * df][col + k * dc] = 0;
            }
        }
    }
}

int main() {
    cout << "Ingrese datos (n m c): ";
    if (!(cin >> filas >> columnas >> largo_c)) return 0;

    // Validación según enunciado: c > 1 y c < 5
    if (largo_c <= 1 || largo_c >= 5) {
        cout << "Error: El largo 'c' debe ser mayor a 1 y menor a 5." << endl;
        return 0;
    }

    // Inicializar matrices
    tablero.assign(filas, vector<int>(columnas, 0));
    mejor_tablero.assign(filas, vector<int>(columnas, 0));

    // El robot siempre empieza a cortar desde alguna de sus esquinas
    vector<pair<int, int>> esquinas = {
        {0, 0}, {0, columnas - 1},
        {filas - 1, 0}, {filas - 1, columnas - 1}
    };

    // Ejecutar backtracking probando iniciar desde todas las esquinas
    for (auto esquina : esquinas) {
        resolver(esquina.first, esquina.second, 1);
    }

    // Imprimir los resultados
    cout << "\nTotal de listones = " << max_listones << "\n\n";

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (mejor_tablero[i][j] == 0) {
                cout << setw(3) << " "; // Imprimir espacio para celdas vacias
            } else {
                cout << setw(3) << mejor_tablero[i][j];
            }
        }
        cout << "\n";
    }

    return 0;
}