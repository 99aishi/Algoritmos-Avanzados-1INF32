#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <algorithm>

#define ITERACIONES 5
#define NUMIND 10
#define TSELECCION 0.3
#define PCASAMIENTO 0.5

using namespace std;

// ----------------------------------------------------------
// Calcula el fitness: mayor cuando se usa más capacidad total
// (equivalente a minimizar el desperdicio total)
// ----------------------------------------------------------
int calculafitness(const vector<int> &cromo,
                   int *paq, int numPaq,
                   int *camiones, int numCam) {
    int sumaUsoTotal = 0;

    // Para cada camión, calculamos uso y verificamos que no se pase
    for (int t = 0; t < numCam; t++) {
        int uso = 0;
        int cap = camiones[t];

        for (int p = 0; p < numPaq; p++) {
            int idx = t * numPaq + p;   // posición en el cromosoma
            if (cromo[idx] == 1) {
                uso += paq[p];
            }
        }

        // Si se pasa de la capacidad, fitness = 0 (solución inválida)
        if (uso > cap) {
            return 0;
        }

        sumaUsoTotal += uso;
    }

    // Si no se asignó nada, no es una buena solución
    if (sumaUsoTotal == 0) return 0;

    // Mientras mayor sea el uso total, menor es el desperdicio
    return sumaUsoTotal;
}

bool aberracion(const vector<int> &ind,
                int *paq, int numPaq,
                int *camiones, int numCam) {
    // 1) Check de capacidad por camión
    for (int t = 0; t < numCam; t++) {
        int uso = 0;
        for (int p = 0; p < numPaq; p++) {
            int idx = t * numPaq + p;
            if (ind[idx] == 1)
                uso += paq[p];
        }
        if (uso > camiones[t]) return true;
    }

    // 2) Cada paquete a lo más en un camión
    for (int p = 0; p < numPaq; p++) {
        int cuenta = 0;
        for (int t = 0; t < numCam; t++) {
            int idx = t * numPaq + p;
            if (ind[idx] == 1) cuenta++;
        }
        if (cuenta > 1) return true;
    }

    return false; // válido
}
void muestrapoblacion(const vector<vector<int>> &poblacion,
                      int *paq,int numPaq,
                      int *camiones,int numCam) {
    for (int i = 0; i < (int)poblacion.size(); i++) {
        cout << "Ind " << i << ": ";
        for (int j = 0; j < (int)poblacion[i].size(); j++) {
            cout << poblacion[i][j] << " ";
        }
        cout << "  fo=" << calculafitness(poblacion[i], paq, numPaq, camiones, numCam) << endl;
    }
}

int decimal(const vector<int> &ind) {
    int num = 0;
    for (int i = 0; i < (int)ind.size(); i++)
        if (ind[i] == 1)
            num += (1 << i); // 2^i
    return num;
}
void mataclon(vector<vector<int>> &poblacion) {
    map<int, vector<int>> indunicos;

    for (int i = 0; i < (int)poblacion.size(); i++) {
        int dec = decimal(poblacion[i]);
        indunicos[dec] = poblacion[i];
    }
    poblacion.clear();
    for (map<int, vector<int>>::iterator it = indunicos.begin();
         it != indunicos.end(); it++) {
        poblacion.push_back(it->second);
    }
}

// ----------------------------------------------------------
// Generar población inicial
// ----------------------------------------------------------
void generapoblacion(vector<vector<int>> &poblacion,
                     int *paq,int numPaq,
                     int *camiones,int numCam) {
    int cont = 0;
    int cromotam = numPaq * numCam;
    srand(time(NULL));

    while (cont < NUMIND) {
        vector<int> individuo(cromotam);
        for (int i = 0; i < cromotam; i++)
            individuo[i] = rand() % 2;

        if (!aberracion(individuo, paq, numPaq, camiones, numCam)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}

// ----------------------------------------------------------
// Calcula probabilidades de supervivencia (ruleta)
// ----------------------------------------------------------
void calculasupervivencia(const vector<vector<int>> &poblacion,
                          vector<int> &supervivencia,
                          int *paq,int numPaq,
                          int *camiones,int numCam) {
    int sumafitness = 0;

    for (int i = 0; i < (int)poblacion.size(); i++)
        sumafitness += calculafitness(poblacion[i], paq, numPaq, camiones, numCam);

    if (sumafitness == 0) {
        supervivencia.assign(poblacion.size(), 1);
        return;
    }

    for (int i = 0; i < (int)poblacion.size(); i++) {
        int superfit = (int)round(100.0 *
                        (double)calculafitness(poblacion[i], paq, numPaq, camiones, numCam) /
                        sumafitness);
        supervivencia.push_back(superfit);
    }
}

// ----------------------------------------------------------
// Carga la ruleta de selección
// ----------------------------------------------------------
void cargaruleta(const vector<int> &supervivencia,int *ruleta) {
    int cont = 0;
    for (int i = 0; i < (int)supervivencia.size() && cont < 100; i++) {
        for (int j = 0; j < supervivencia[i] && cont < 100; j++) {
            ruleta[cont++] = i;
        }
    }
    while (cont < 100) {
        ruleta[cont++] = -1;
    }
}

// ----------------------------------------------------------
// Selección por ruleta
// ----------------------------------------------------------
void seleccion(const vector<vector<int>> &poblacion,
               vector<vector<int>> &padres,
               int *paq,int numPaq,
               int *camiones,int numCam) {
    int ruleta[100];
    for (int i = 0; i < 100; i++) ruleta[i] = -1;

    vector<int> supervivencia;
    calculasupervivencia(poblacion, supervivencia, paq, numPaq, camiones, numCam);
    cargaruleta(supervivencia, ruleta);

    int npadres = (int)round(poblacion.size() * TSELECCION);

    for (int i = 0; i < npadres; i++) {
        int ticket = rand() % 100;
        if (ruleta[ticket] != -1)
            padres.push_back(poblacion[ruleta[ticket]]);
    }
}

// ----------------------------------------------------------
// Cruce de dos padres
// ----------------------------------------------------------
void creahijo(const vector<int> &padre,
              const vector<int> &madre,
              vector<int> &hijo) {
    int posi = (int)round(padre.size() * PCASAMIENTO);

    hijo.clear();
    for (int i = 0; i < posi; i++)
        hijo.push_back(padre[i]);
    for (int i = posi; i < (int)madre.size(); i++)
        hijo.push_back(madre[i]);
}

// ----------------------------------------------------------
// Casamiento: genera hijos válidos
// ----------------------------------------------------------
void casamiento(vector<vector<int>> &poblacion,
                const vector<vector<int>> &padres,
                int *paq,int numPaq,
                int *camiones,int numCam) {

    for (int i = 0; i < (int)padres.size(); i++) {
        for (int j = 0; j < (int)padres.size(); j++) {
            if (i != j) {
                vector<int> hijo;
                creahijo(padres[i], padres[j], hijo);
                if (!aberracion(hijo, paq, numPaq, camiones, numCam))
                    poblacion.push_back(hijo);
            }
        }
    }
}

void regenerapoblacion(vector<vector<int>> &poblacion,
                       int *paq,int numPaq,
                       int *camiones,int numCam){
    mataclon(poblacion);

    sort(poblacion.begin(), poblacion.end(),
         [paq, camiones, numPaq, numCam](const vector<int>& a, const vector<int>& b) {
            int fa = calculafitness(a, paq, numPaq, camiones, numCam);
            int fb = calculafitness(b, paq, numPaq, camiones, numCam);
            return fa > fb; // mayor fitness primero
         });

    if ((int)poblacion.size() > NUMIND)
        poblacion.erase(poblacion.begin() + NUMIND, poblacion.end());
}

int muestramejor(const vector<vector<int>> &poblacion,
                 int *paq,int numPaq,
                 int *camiones,int numCam){
    int mejor = 0;

    for (int i = 1; i < (int)poblacion.size(); i++)
        if (calculafitness(poblacion[i],paq,numPaq,camiones,numCam) >
            calculafitness(poblacion[mejor],paq,numPaq,camiones,numCam))
            mejor = i;

    int mejorFit = calculafitness(poblacion[mejor],paq,numPaq,camiones,numCam);
    cout << endl << "La mejor solucion tiene fitness (peso total usado): "
         << mejorFit << endl;
    cout << "Cromosoma: ";
    for (int i = 0; i < (int)poblacion[mejor].size(); i++)
        cout << poblacion[mejor][i] << " ";
    cout << endl;

    // Mostrar asignación por camión y desperdicio
    int desperdicioTotal = 0;
    int sumaCap = 0;

    cout << "\nAsignación de paquetes a camiones:" << endl;
    for (int t = 0; t < numCam; t++) {
        int uso = 0;
        cout << "Camion " << t+1 << " (capacidad " << camiones[t] << "): ";
        for (int p = 0; p < numPaq; p++) {
            int idx = t * numPaq + p;
            if (poblacion[mejor][idx] == 1) {
                cout << "P" << (p+1) << " ";
                uso += paq[p];
            }
        }
        int desp = camiones[t] - uso;
        desperdicioTotal += desp;
        sumaCap += camiones[t];
        cout << "  [uso=" << uso << ", desperdicio=" << desp << "]" << endl;
    }

    cout << "DESPERDICIO TOTAL = " << desperdicioTotal
         << " de " << sumaCap << " kg de capacidad." << endl;

    return desperdicioTotal;
}

void asignacionCamionesAG(int *paq,int numPaq,
                          int *camiones,int numCam) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion,paq,numPaq,camiones,numCam);
    cout << "Población inicial:" << endl;
    muestrapoblacion(poblacion,paq,numPaq,camiones,numCam);
    for (int it = 0; it < ITERACIONES; it++) {
        cout << "\nITERACION " << it+1 << " ----------------------" << endl;
        vector<vector<int>> padres;
        seleccion(poblacion,padres,paq,numPaq,camiones,numCam);
        casamiento(poblacion,padres,paq,numPaq,camiones,numCam);
        regenerapoblacion(poblacion,paq,numPaq,camiones,numCam);
        muestrapoblacion(poblacion,paq,numPaq,camiones,numCam);
    }
    muestramejor(poblacion,paq,numPaq,camiones,numCam);
}

int main() {
    // Ejemplo TUPIACU:
    // 6 paquetes
    // Pesos: 150, 100, 180, 50, 120, 10
    int paq[] = {150, 100, 80, 50, 120, 10};
    int numPaq = sizeof(paq)/sizeof(paq[0]);

    // 4 camiones
    // Capacidades: 250, 200, 200, 100
    int camiones[] = {250, 200, 200, 200};
    int numCam = sizeof(camiones)/sizeof(camiones[0]);

    asignacionCamionesAG(paq,numPaq,camiones,numCam);

    return 0;
}
