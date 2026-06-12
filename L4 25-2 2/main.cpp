/*
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 6/12/2026, 12:56 AM
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>
#define ITERACIONES 1000
#define alfa 0.30
#define k 20
using namespace std;
struct Ciudad {
    char nombre;
    int costo;
    int asignado;
};

struct Servicio {
    int codigo;
    vector<Ciudad> precios;
    int precio;
};
bool compara(Ciudad a, Ciudad b) {
    return a.nombre<b.nombre;
}
bool comparaCiudades(Ciudad c1, Ciudad c2) {
    return c1.costo < c2.costo;
}
bool comparaServCiudad(Servicio s1, Servicio s2) {
    return s1.precio < s2.precio;
}

int buscaind(vector<Ciudad> v,double maxrcl) {
    int ind=0;
    for (auto vs: v) {
        if (vs.costo <=maxrcl) {
            ind++;
        }
    }
    return ind;
}
int buscaindserv(vector<Servicio> s,double maxrcl) {
    int ind=0;
    for (auto vs: s) {
        if (vs.precio <=maxrcl) {
            ind++;
        }
    }
    return ind;
}


void ordenarServiciosCiudad(vector<Servicio>&servicios, char ciudad) {

    for (int i=0;i<servicios.size();i++) {
        for (int j=0;j<servicios[i].precios.size();j++) {
            if (servicios[i].precios[j].nombre == ciudad) {
                servicios[i].precio=servicios[i].precios[j].costo;
            }
        }
    }
    sort(servicios.begin(),servicios.end(),comparaServCiudad);
}

void grasp(vector<Ciudad> ciudades,vector<Servicio>servicios,int n , int m) {
    int mejor = INT_MAX;
    // ordenarServiciosCostos(servicios);

    // sort(servicios.begin(),servicios.end(),compara);

    vector<Ciudad> mejorSolu;

    srand(time(NULL));
    for (int i = 0; i < ITERACIONES; i++) {
        vector<Ciudad> resultado;
        vector<Ciudad> vciudades;
        vector<Servicio>vserv;

        vciudades.insert(vciudades.begin(),ciudades.begin(),ciudades.begin()+n);
        vserv.insert(vserv.begin(),servicios.begin(),servicios.begin()+m);
        sort(vciudades.begin(),vciudades.end(),comparaCiudades);
        int suma=0;
        int cont=0;
        while (!vciudades.empty()) {
            int betac=vciudades[0].costo;
            int tauc=vciudades[vciudades.size()-1].costo;
            double rclc=betac + alfa*(tauc-betac);
            int indc=buscaind(vciudades,rclc);
            int indac=rand()%indc;

            ordenarServiciosCiudad(vserv,vciudades[indac].nombre);

            int betas=vserv[0].precio;
            int taus=vserv[vserv.size()-1].precio;
            double rcls=betas+alfa*(taus-betas);
            int inds=buscaindserv(vserv,rcls);
            int indas=rand()%inds;
            if (vciudades[indac].asignado==0 and vciudades[indac].costo+vserv[indas].precio<=k) {
                vciudades[indac].asignado=vserv[indac].codigo;
                resultado.push_back(vciudades[indac]);
                suma+=vciudades[indac].costo+vserv[indac].precio;
                cont++;
            }
            vciudades.erase(vciudades.begin()+indac);
        }

        if (cont==4 and suma<mejor) {
            mejor=suma;
            mejorSolu.clear();
            mejorSolu.insert(mejorSolu.begin(),resultado.begin(),resultado.end());
        }


    }
    sort(mejorSolu.begin(),mejorSolu.end(),compara);
    for (int i=0;i<mejorSolu.size();i++) {
        cout << mejorSolu[i].nombre << " " <<mejorSolu[i].asignado <<endl;

    }
    cout << "Total:"<<mejor;

}
int main(int argc, char **argv) {

    vector<Ciudad> ciudades={{'A',16,0},{'B',15,0},{'C',12,0},{'D',18,0}};
    vector<Servicio>servicios={
        {1,{{'A',3},{'B',4},{'C',5},{'D',5}},0},
        {2,{{'A',5},{'B',2},{'C',3},{'D',4}},0},
        {3,{{'A',7},{'B',5},{'C',4},{'D',2}},0},
        {4,{{'A',8},{'B',7},{'C',5},{'D',1}},0},
        {5,{{'A',4},{'B',3},{'C',7},{'D',5}},0}
    };
    int n =ciudades.size();
    int m=servicios.size();

    grasp(ciudades,servicios,n,m);



    return 0;
}