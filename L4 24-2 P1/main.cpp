/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 6/13/2026, 1:04 AM
 */

#include <algorithm>
#include <iostream>
#include <vector>

#define ITERACIONES 1000
#define alfa 0.30
using namespace std;
struct Pesos {
    int peso;
};
struct Paquete {
    vector<Pesos>pesos;
    int acumulados;
    Paquete() {
        acumulados = 0;
    }
};


bool compara(Pesos a, Pesos b) {
    return a.peso>b.peso;
}
int buscaInd(vector<Pesos> pesos, double rcl) {
    int j=0;
    for (int i = 0; i < pesos.size(); i++) {
        if (pesos[i].peso>=rcl)
            j++;
    }
    return j;
}
void grasp(vector<Pesos> pesos, int n, vector<Paquete> paquetes) {

    int cantPaq= INT_MAX;
    vector<Paquete>vmejorSolu;
    vector<Paquete>vsoluciones;
    vsoluciones.push_back(Paquete());
    srand(time(NULL));

    for (int i = 0; i < ITERACIONES; i++) {

        // vector<Paquete>vpaquetes;
        vector<Pesos>vpesos;
        vpesos.insert(vpesos.begin(), pesos.begin(), pesos.begin() + n);

        sort(vpesos.begin(), vpesos.end(),compara);

        while (!vpesos.empty()) {
            int betapesos=vpesos[0].peso;
            int tautpesos=vpesos[vpesos.size()-1].peso;
            double rcl=betapesos - alfa*(betapesos-tautpesos);
            int ind=buscaInd(vpesos,rcl);
            int indd=rand()%ind;

            bool asignado=false;
            for (int j=0;j<vsoluciones.size();j++) {
                if (vsoluciones[j].acumulados+vpesos[indd].peso<=10) {
                    vsoluciones[j].acumulados+=vpesos[indd].peso;
                    vsoluciones[j].pesos.push_back(vpesos[indd]);
                    asignado=true;
                    break;
                }
            }
            if (!asignado) {
                Paquete pq;
                pq.acumulados+=vpesos[indd].peso;
                pq.pesos.push_back(vpesos[indd]);
                vsoluciones.push_back(pq);
            }
            vpesos.erase(vpesos.begin()+indd);
        }

        if (cantPaq> vsoluciones.size()) {
            cantPaq=vsoluciones.size();
            vmejorSolu=vsoluciones;
        }


    }

    for (int i = 0; i < vmejorSolu.size(); i++) {
        cout << "Acumulado "<<vmejorSolu[i].acumulados;
        for (int j=0;j<vmejorSolu[i].pesos.size();j++) {
            cout << " " <<vmejorSolu[i].pesos[j].peso <<" ";
        }
        cout << endl;
    }

}


int main(int argc, char **argv) {

    vector<Pesos> pesos{{4}, {8},{ 1},{ 4},{ 2}, {1}};
    vector<Paquete> paquetes;
    paquetes.push_back(Paquete());
    int n = pesos.size();

    grasp(pesos,n, paquetes);



    return 0;
}