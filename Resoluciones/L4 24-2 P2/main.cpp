/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 6/13/2026, 2:33 AM
 */

#include <algorithm>
#include <iostream>
#include <vector>
#define ITERACIONES 10000
#define alfa 0.40
using namespace std;


struct Lugar {
    int indice;
    char nombre;
    int pago;
};
bool compara(Lugar a, Lugar b) {
    return a.pago>b.pago;
}
int buscarInd(vector<Lugar> pos , double rcl) {
    int j=0;
    for (int i=0;i<pos.size();i++) {
        if (pos[i].pago>=rcl) {
            j++;
        }
    }
    return j;
}

void GRASP(vector<vector<Lugar>> matriz, int n) {
    vector<Lugar> mejorSolu;
    int mejorSuma = INT_MIN;
    srand(time(NULL));
    vector<Lugar>solucion;
    for (int i=0 ; i<ITERACIONES ; i++) {
        int suma=0;
        int pos=0;
        while (true) {
            vector<Lugar> vposicion;
            vposicion.insert(vposicion.begin(),matriz[pos].begin(),matriz[pos].end());
            if (vposicion.empty())
                break;
            sort(vposicion.begin(),vposicion.end(),compara);

            int beta = vposicion[0].pago;
            int tau=vposicion[vposicion.size()-1].pago;
            double rcl = beta - alfa*(beta-tau);
            int ind = buscarInd(vposicion,rcl);
            int index = rand()%ind;

            solucion.push_back(vposicion[index]);
            suma+=vposicion[index].pago;
            pos = vposicion[index].indice;
        }

        if (suma>mejorSuma) {
            mejorSolu=solucion;
            mejorSuma=suma;
        }

    }

    cout << mejorSuma << endl;
    for (int i=0;i<mejorSolu.size();i++) {
        cout << mejorSolu[i].nombre << " ";
    }
}


int main(int argc, char **argv) {
// A B C D E F G H

 //   {0,20,30,40,40,40,10,50},
   // {0,0,30,40,0,40,0,50}

    Lugar A ={0,'A',0};
    Lugar B = {1,'B',20};
    Lugar C={2,'C',30};
    Lugar D={3,'D',40};
    Lugar E={4,'E',40};
    Lugar F={5,'F',40};
    Lugar G={6,'G',10};
    Lugar H={7,'H',50};



    vector<vector<Lugar>> matriz;
    matriz.push_back({B,C,D,E,H});
    matriz.push_back({C,D,F,H});
    matriz.push_back({F});
    matriz.push_back({});
    matriz.push_back({C,F,G});
    matriz.push_back({});
    matriz.push_back({});
    matriz.push_back({});
    int n= matriz.size();
    GRASP(matriz,n);
    return 0;
}