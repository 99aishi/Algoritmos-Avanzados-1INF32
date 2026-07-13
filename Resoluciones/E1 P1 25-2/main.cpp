#include <iostream>
#include <vector>

using namespace std;

// Estructura para almacenar la información de cada región
struct Region {
    int target;         // Suma objetivo de la región
    int size;           // Cantidad de celdas que componen la región
    int current_sum;    // Suma actual de los números colocados
    int filled_cells;   // Cantidad de celdas ya llenadas en esta región
    vector<bool> used;  // Arreglo para marcar los números ya usados en la región

    Region(int n) {
        target = 0;
        size = 0;
        current_sum = 0;
        filled_cells = 0;
        used.assign(n + 1, false);
    }
};

// Variables globales para simplificar el paso de parámetros en el backtracking
int n, k;
vector<vector<int>> board;
vector<vector<int>> cell_region; // Mapea la coordenada (r,c) al ID de su región
vector<Region> regions;
vector<vector<bool>> row_used;   // row_used[r][num] = true si 'num' está en la fila 'r'
vector<vector<bool>> col_used;   // col_used[c][num] = true si 'num' está en la columna 'c'

/*
 * Función: isValid
 * Lógica: Verifica si es seguro colocar el número 'num' en la celda (r, c).
 * Para ello comprueba:
 * 1. Que el número no esté en la misma fila.
 * 2. Que el número no esté en la misma columna.
 * 3. Que el número no haya sido usado ya dentro de la región correspondiente.
 * 4. Que al sumar el número a la región, no se sobrepase la suma objetivo.
 * 5. Si es la última celda disponible en la región, la suma debe ser exactamente la suma objetivo.
 */
bool isValid(int r, int c, int num) {
    if (row_used[r][num]) return false;
    if (col_used[c][num]) return false;

    int reg_id = cell_region[r][c];
    if (regions[reg_id].used[num]) return false;

    int new_sum = regions[reg_id].current_sum + num;
    if (new_sum > regions[reg_id].target) return false;

    int new_filled = regions[reg_id].filled_cells + 1;
    // Si estamos colocando el último número de la región, la suma debe cuadrar exacto
    if (new_filled == regions[reg_id].size) {
        if (new_sum != regions[reg_id].target) return false;
    }

    return true;
}

/*
 * Función: solve
 * Lógica: Utiliza el enfoque de Backtracking (Vuelta atrás) para intentar colocar
 * números celda por celda (de izquierda a derecha, y de arriba hacia abajo).
 * Si logra llenar la cuadrícula (r == n), retorna verdadero.
 * Para cada celda, prueba colocar los números del 1 al n. Si el número es válido
 * (verificado con isValid), actualiza las estructuras de estado y avanza a la siguiente celda.
 * Si ese camino no lleva a una solución, deshace los cambios (backtrack) y prueba con el siguiente número.
 */
bool solve(int r, int c) {
    // Caso base: Si superamos la última fila, ya completamos el tablero con éxito
    if (r == n) return true;

    // Si llegamos al final de una columna, saltamos a la siguiente fila
    if (c == n) return solve(r + 1, 0);

    // Intentamos colocar los números del 1 al n
    for (int num = 1; num <= n; ++num) {
        if (isValid(r, c, num)) {
            int reg_id = cell_region[r][c];

            // 1. Tomar la decisión (Colocar el número)
            board[r][c] = num;
            row_used[r][num] = true;
            col_used[c][num] = true;
            regions[reg_id].used[num] = true;
            regions[reg_id].current_sum += num;
            regions[reg_id].filled_cells++;

            // 2. Explorar el siguiente paso
            if (solve(r, c + 1)) return true;

            // 3. Deshacer la decisión (Backtrack) si el camino explorado falla
            board[r][c] = 0;
            row_used[r][num] = false;
            col_used[c][num] = false;
            regions[reg_id].used[num] = false;
            regions[reg_id].current_sum -= num;
            regions[reg_id].filled_cells--;
        }
    }
    // Si ningún número funciona en esta celda, retornamos falso para hacer backtrack
    return false;
}

/*
 * Función: main
 * Lógica: Punto de entrada del programa. Se encarga de la lectura de los datos de entrada
 * según el formato requerido (n y k seguidos de la descripción de las regiones).
 * Inicializa las matrices y vectores necesarios. Luego, invoca al algoritmo recursivo de
 * Backtracking (solve). Si el algoritmo encuentra solución la imprime, en caso contrario
 * imprime el mensaje estricto requerido de "1. NO HAY SOLUCION".
 */
int main() {
    // Leer n y k. Si no hay entrada, terminar.
    if (!(cin >> n >> k)) return 0;

    // Inicialización de estructuras
    board.assign(n, vector<int>(n, 0));
    cell_region.assign(n, vector<int>(n, 0));
    row_used.assign(n, vector<bool>(n + 1, false));
    col_used.assign(n, vector<bool>(n + 1, false));

    // Lectura de las regiones
    for (int i = 0; i < k; ++i) {
        Region reg(n);
        cin >> reg.target >> reg.size;
        for (int j = 0; j < reg.size; ++j) {
            int r, c;
            cin >> r >> c;
            cell_region[r][c] = i; // Mapeamos las coordenadas a esta región i
        }
        regions.push_back(reg);
    }

    // Invocamos el Backtracking desde la celda (0,0)
    if (solve(0, 0)) {
        // Imprimir solución si fue hallada
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << board[i][j] << (j == n - 1 ? "" : " ");
            }
            cout << "\n";
        }
    } else {
        // Imprimir si no hay solución exactamente como lo indica el enunciado
        cout << "1. NO HAY SOLUCION\n";
    }

    return 0;
}