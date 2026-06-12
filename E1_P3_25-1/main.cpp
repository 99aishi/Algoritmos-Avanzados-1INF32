/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/17/2026, 10:50 PM
 */

#include <algorithm>
#include <iostream>
#include <vector>

#include "Linea.h"
#include "Orden.h"
using namespace std;
bool compara(Linea a, Linea b) {
    if (a.tiempoAcumulado==b.tiempoAcumulado) {
        return a.id < b.id;
    }
    return a.tiempoAcumulado < b.tiempoAcumulado;
}
bool comparaID(Linea a, Linea b) {
        return a.id < b.id;
}
int main(int argc, char **argv) {
    vector<Orden> ordenes;
    int M=5;
    int N=20;
    int k=0;
    while (k<N) {
        Orden  orden;
        int id, cantidad;
        cin >> id >> cantidad;
        orden = Orden(id, cantidad,cantidad*6);
        ordenes.push_back(orden);
        k++;
    }
    cout << ordenes.size() << endl;
    vector<Linea> lineas(M);
    for (int i=0; i<M; i++) {
        lineas[i].id=i+1;
        lineas[i].tiempoAcumulado=0;
    }

    sort(lineas.begin(), lineas.end(),compara);

    for (int i=0; i<N; i++) {
        lineas[0].tiempoAcumulado+=ordenes[i].tiempo;
        lineas[0].ordenes.push_back(ordenes[i]);
        sort(lineas.begin(), lineas.end(),compara);
    }
    sort(lineas.begin(), lineas.end(),comparaID);
    int mayor=0;
    for (int i=0; i<M; i++) {
        cout << "Linea "<< lineas[i].id <<" :";
        for (int j=0; j<lineas[i].ordenes.size(); j++) {
            cout << " Orden" << lineas[i].ordenes[j].id<<"("<<lineas[i].ordenes[j].cantidad<<") ";
        }
        if (mayor<lineas[i].tiempoAcumulado)
            mayor =lineas[i].tiempoAcumulado;
        cout<< " "<< lineas[i].tiempoAcumulado<<endl;
        // cout << endl;
    }
    cout << "Makespan :" <<mayor;
    return 0;
}