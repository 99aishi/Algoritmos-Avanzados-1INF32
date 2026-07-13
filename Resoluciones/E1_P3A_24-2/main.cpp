/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/18/2026, 11:12 AM
 */

#include <algorithm>
#include <iostream>
#include <vector>

#include "Conjunto.h"
#include "SubConjunto.h"
using namespace std;

void calcularDisponibles(Conjunto conjunto,SubConjunto &sub) {
    int incluidos = 0;
    int faltantes=0;
    for (int i = 0; i < sub.lista.size(); ++i) {
        for (int j = 0; j < conjunto.lista.size(); ++j) {
            if (sub.lista[i] == conjunto.lista[j]) {
                if (conjunto.incluidos[j]==false) {
                    faltantes++;
                }else {
                    incluidos++;
                }
            }
        }
    }
    sub.cantIncluidos = incluidos;
    sub.cantFaltantes = faltantes;

}

void incluirSub(Conjunto &conjunto,SubConjunto &sub) {
    for (int i = 0; i < sub.lista.size(); ++i) {
        for (int j = 0; j < conjunto.lista.size(); ++j) {
            if (sub.lista[i] == conjunto.lista[j]) {
                if (conjunto.incluidos[j]==false) {
                    conjunto.incluidos[j] = true;
                }
            }
        }
    }
    calcularDisponibles(conjunto,sub);
}
bool compara(const SubConjunto &a,const SubConjunto &b) {

    if (a.cantFaltantes == b.cantFaltantes) {

        if (a.cantIncluidos==b.cantIncluidos) {
            return a.id < b.id;
        }

        return a.cantIncluidos<b.cantIncluidos;
    }

    return a.cantFaltantes > b.cantFaltantes;

}
bool validarFin(const Conjunto &conjunto, int i) {

    if (i>=conjunto.incluidos.size()) {
        return true;
    }
    if (conjunto.incluidos[i]==false) {
        return false;
    }

    return validarFin(conjunto, i+1);
}
int main(int argc, char **argv) {

    Conjunto conjunto;

    vector<int> lista={1,2,3,4,5,6,7};
    vector<bool> incl(lista.size(),false);

    conjunto.lista=lista;
    conjunto.incluidos=incl;


    vector<SubConjunto> subs;
    subs.push_back(SubConjunto(1,{1,2,3}));
    subs.push_back(SubConjunto(2,{2,4,5}));
    subs.push_back(SubConjunto(3,{3,5,6}));
    subs.push_back(SubConjunto(4,{6,7}));


    // for (SubConjunto &sub : subs) {
    //     calcularDisponibles(conjunto,sub);
    // }
    sort(subs.begin(),subs.end(),compara);
    vector<SubConjunto> solu;

    for (int i = 0; i < subs.size(); ++i) {
        if (validarFin(conjunto,0)) {
            break;
        }
        solu.push_back(subs[0]);
        incluirSub(conjunto,subs[0]);
        //subs.erase(subs.begin());
        for (SubConjunto &as : subs) {
            calcularDisponibles(conjunto,as);
        }
        sort(subs.begin(),subs.end(),compara);
    }
    for (SubConjunto &sub : solu) {
        cout << sub.id << " :";
        for (int j = 0; j < sub.lista.size(); ++j) {
            cout << sub.lista[j] << " ";
        }
        cout << endl;
    }

    return 0;
}