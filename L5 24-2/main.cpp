/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 6/27/2026, 1:08 AM
 */

#include <cmath>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>

#define NUMIND 20
#define PCASAMIENTO 0.80
#define ITERACIONES 1000
#define TMUTACION 0.10


using namespace std;


bool aberracion(vector<int> individuo,vector<vector<int>> ganancia, int n, int m) {

    //Cada tarea debe ser asignado maximo un empleado diferente
    //i = tarea n, j = empleado m
    for (int i=0;i<n;i++) {
        int cont =0;
        for (int j=0;j<m;j++) {
            int idx = i*m + j;
            if (individuo[idx] == 1) cont ++ ;

        }
        if (cont != 1 ) return true;
    }
    //maximo un empleado por tarea

    for (int j = 0; j < m; j++) {
        int cont = 0;
        for (int i = 0; i < n; i++) {
            int idx = i + m * j;
            if (individuo[idx] == 1) cont++;
        }
        if (cont != 1) return true;
    }


    return false;
}


void generaPoblacion(vector<vector<int>> ganancia, int n, int m,vector<vector<int>> &poblacion) {
    int cont =0;
    int cromotam=n*m;
    srand(time(NULL));

    while (cont < NUMIND) {
        vector<int> individuo(cromotam, 0); // Inicializamos todo en 0
        vector<int> empleados = {0, 1, 2, 3, 4}; // Representan a los 5 empleados

        // Mezclamos a los empleados aleatoriamente
        random_shuffle(empleados.begin(), empleados.end());

        // Asignamos exactamente 1 empleado por tarea
        for (int i = 0; i < n; i++) {
            int j = empleados[i]; // Empleado asignado a la tarea i
            int idx = i * m + j;
            individuo[idx] = 1;
        }
        if (!aberracion(individuo,ganancia,n,m)) {
            poblacion.push_back(individuo);
            cont++;
        }

    }
    cout << " xd ";

}
int calcularFitness(vector<int> poblacion, int n, int m,vector<vector<int>> ganancia) {

    int suma= 0;

    for (int i=0;i<n;i++) {

        for (int j=0;j<m;j++) {
            int idx = i*m + j;
            if (poblacion[idx]==1)
                suma += ganancia[i][j];
        }

    }
    return suma;


}
void muestraPoblacion(vector<vector<int>> poblacion,vector<vector<int>> ganancia, int n, int m) {
    for (int i=0;i<poblacion.size();i++) {
        for (int j=0;j<poblacion[i].size();j++) {
            cout << poblacion[i][j] << " ";
        }
        cout << "fo="<< calcularFitness(poblacion[i],n,m,ganancia);
        cout << endl;
    }
}

void
    calcularSupervivencia(vector<vector<int>> poblacion,vector<int> &supervivencia,
        vector<vector<int>>ganancia,int n,int m) {

    int sumaFitness = 0;

    for (int i=0;i<poblacion.size();i++) {
        sumaFitness += calcularFitness(poblacion[i],n,m,ganancia);
    }
    for (int i=0;i<poblacion.size();i++) {
        int superfit= (int)round(100.0 * (double)calcularFitness(poblacion[i],n,m,ganancia)/sumaFitness);
        supervivencia.push_back(superfit);
    }


}
void cargaRuleta(vector<int> supervivencia,vector<int>& ruleta) {
    int cont =0;
    for (int i=0;i<supervivencia.size() and cont<100;i++) {
        for (int j=0;j<supervivencia[i] and cont<100;j++) {
            ruleta[cont++]=i;
        }
    }
    while (cont<100) {
        ruleta[cont++]=-1;
    }
}
void seleccion(vector<vector<int>>poblacion,
                vector<vector<int>>& padres,
                vector<vector<int>> ganancia,
                int n, int m) {
    //seleccion por ruleta
    vector<int>ruleta(100,-1);

    vector<int> supervivencia;
    calcularSupervivencia(poblacion,supervivencia,ganancia,n,m);
    cargaRuleta(supervivencia,ruleta);

    int npadres=poblacion.size();

    for (int i=0;i<npadres;i++) {
        int ticket= rand()%100;
        if (ruleta[i]!=-1) {
            padres.push_back(poblacion[ruleta[ticket]]);
        }
    }

}
void crearHijo(vector<int>padre, vector<int>madre,vector<int>&hijo) {

    int posi= (int)round(padre.size()*PCASAMIENTO);
    hijo.clear();

    for (int i=0;i<posi;i++) {
        hijo.push_back(padre[i]);
    }
    for (int i=posi;i<padre.size();i++) {
        hijo.push_back(madre[i]);
    }


}
void casamiento(vector<vector<int>>& poblacion,vector<vector<int>> padres,vector<vector<int>> ganancia,int n,int m ) {

    for (int i=0;i<padres.size();i++) {
        for (int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                crearHijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,ganancia,n,m)) {
                    poblacion.push_back(hijo);
                }
            }
        }
    }

}
int decimal(vector<int>ind) {
    int num=0;
    for (int i=0;i<ind.size();i++) {
        num+=ind[i]* pow(2,i);
    }
    return num;
}
void mataclon(vector<vector<int>> &poblacion) {
    map<int,vector<int>> indunicos;

    for (int i=0;i<poblacion.size();i++) {
        int dec=decimal(poblacion[i]);
        indunicos[dec]= poblacion[i];
    }
    poblacion.clear();
    for (map<int,vector<int>>::iterator it=indunicos.begin();it!=indunicos.end();it++) {
        poblacion.push_back(it->second);
    }

}
void mutacion(vector<vector<int>>& poblacion,vector<vector<int>> padres,vector<vector<int>> ganancia,int n,int m) {
    int nmutaciones= round(padres[0].size()*TMUTACION);

    for (int i=0;i<padres.size();i++) {
        int cont=0;
        while (cont<nmutaciones) {
            int gen = rand()%padres[i].size();
            if (padres[i][gen]==1) padres[i][gen]=0;
            else padres[i][gen]=1;
            cont++;
        }
        if (!aberracion(padres[i],ganancia,n,m)) {
            poblacion.push_back(padres[i]);
        }
    }



}

void regenerapoblacion(vector<vector<int>>&poblacion,vector<vector<int>> ganancia,int n,int m) {
    mataclon(poblacion);

    sort(poblacion.begin(),poblacion.end(),
        [ganancia,n,m](const vector<int>&a,const vector<int>&b) {
            int fa= calcularFitness(a,n,m,ganancia);
            int fb= calcularFitness(b,n,m,ganancia);
            return fa>fb;//mayor fitness primero
        });

    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}
int muestraMejor(vector<vector<int>> poblacion,vector<vector<int>> ganancia,int n,int m)
{
    int mejor=0;
    for (int i=1;i<poblacion.size();i++) {
        if (calcularFitness(poblacion[i],n,m,ganancia)>calcularFitness(poblacion[mejor],n,m,ganancia)) {
            mejor=i;
        }
    }

    int mejorFit = calcularFitness(poblacion[mejor],n,m,ganancia);
    cout << endl << " La mejor solucion tiene fitness: " << mejorFit << endl;
    cout << "Cromosoma: ";
    for (int i=0;i<poblacion[mejor].size();i++) {
        cout << poblacion[mejor][i] << " ";
    }
    cout << endl;

    return mejorFit;
}
void asignacionTareas(vector<vector<int>> ganancia, int n, int m) {

    vector<vector<int>> poblacion;
    generaPoblacion(ganancia,n,m,poblacion);
    cout << "Poblacion inicial:" << endl;
    muestraPoblacion(poblacion,ganancia,n,m);

    for (int it =0 ; it < ITERACIONES ; it++) {
        cout << "-----------------------------------------------------------"<<endl;
        vector<vector<int>> padres;
        seleccion(poblacion,padres,ganancia,n,m);
        casamiento(poblacion,padres,ganancia,n,m);
        mataclon(poblacion);
        mutacion(poblacion,padres,ganancia,n,m);
        //mataclon(poblacion);
        regenerapoblacion(poblacion,ganancia,n,m);
        muestraPoblacion(poblacion,ganancia,n,m);
    }
    muestraMejor(poblacion,ganancia,n,m);



}

int main(int argc, char **argv) {
    int n=5,m=5;
    vector<vector<int>> ganancia=
        {{9,2,7,8,6},
        {6,4,3,7,8},
        {5,8,1,8,3},
        {7,6,9,4,2},
        {8,6,7,5,9}};


    asignacionTareas(ganancia,n,m);

    return 0;
}

