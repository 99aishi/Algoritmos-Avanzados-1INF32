/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/17/2026, 7:22 PM
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

double calcularDistancias(vector<double> a, vector<double> b) {
    int p= pow(b[0]-a[0],2);
    int q= pow(b[1]-a[1],2);
    return sqrt(pow(a[0]-b[0],2) + pow(a[1]-b[1],2));
}
void llenarDistancias(vector<vector<double>>& distancias,vector<vector<double>> coordenadas) {
    for (int i = 0; i < distancias.size(); i++) {
        for (int j = 0; j < distancias[i].size(); j++) {
            distancias[i][j]= calcularDistancias(coordenadas[i],coordenadas[j]);
        }
    }
}
bool esVisitado(int i, vector<int>visitas) {
    for (int j = 0; j < visitas.size(); j++) {
        if (visitas[j] == i) {
            return true;
        }
    }
    return false;
}
double calcularSuma(vector<vector<double>>distancias,vector<int>visitas,int pos,int ind) {
    double suma=0;
    if (ind+1==visitas.size()) {
        return distancias[pos][0];
    }
    suma = distancias[pos][visitas[ind+1]];
    suma +=calcularSuma(distancias,visitas,visitas[ind+1],ind+1);
    return suma;

}
bool backtracking(vector<vector<double>>distancias,vector<int>visitas, double &optimo,int pos, int n, vector<int>&respuesta,int &cont) {

    if (visitas.size()==n) {
        double valor=0;
        cont++;
        if (cont==46) {
            cout << " . ";
        }
        valor=calcularSuma(distancias,visitas,0,0);
        if (valor > 16.8 and valor < 17) {
            cout << " . ";
        }
        if (valor < optimo) {
            optimo=valor;
            respuesta=visitas;
        }
        return false;
    }
    for (int i = 0; i < n; i++) {
        if (!esVisitado(i,visitas)) {
            visitas.push_back(i);
            //visitas[i]=pos;
            if (backtracking(distancias,visitas,optimo,pos+1,n,respuesta,cont)) {

            }
            visitas.pop_back();

        }
    }
    return false;

}


int main(int argc, char **argv) {

    vector<vector<double>> coordenadas;
    coordenadas.push_back({2,3});
    coordenadas.push_back({1,1});
    coordenadas.push_back({1,5.5});
    coordenadas.push_back({6,3});
    coordenadas.push_back({2.5,1});
    coordenadas.push_back({3.5,6});

    vector<vector<double>> distancias(coordenadas.size(), vector<double>(coordenadas.size(), 0));
    llenarDistancias(distancias,coordenadas);

    for (int i = 0; i < distancias.size(); i++) {
        for (int j = 0; j < distancias[i].size(); j++) {
            cout << setw(5)<<setprecision(3)<<distancias[i][j] << " ";
        }
        cout << endl;
    }

    double optimo=9999;
    int pos=1;
    vector<bool> esVisitado(coordenadas.size(),false);
    vector<int> visitas;
    visitas.push_back(0);
    vector<int>respuesta;
    int cont=0;
    backtracking(distancias,visitas,optimo,pos,distancias.size(),respuesta,cont);
    for (int i=0;i<respuesta.size();i++) {
        cout << respuesta[i] << " ";
    }
    cout << endl;
    cout<<setprecision(3) << optimo<<" "<<cont << endl;
    return 0;
}
//2.69+2.55+3.91+4+4.03+2.06