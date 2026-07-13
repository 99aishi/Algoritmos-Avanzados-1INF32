/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/8/2026, 11:58 PM
 */

#include <algorithm>
#include <iostream>
#include <string>
#include "Pais.h"
#include "Color.h"

using namespace std;
bool comparaVecinos(Pais a, Pais b) {

    if (a.vecinos.size() == b.vecinos.size()) {
        return a.num>b.num;
    }
    return a.vecinos.size()>b.vecinos.size();
}
bool esPaisExcluido(Color color, string pais) {
    for (int i=0;i<color.antis.size();i++) {
        if (pais==color.antis[i]) {
            return true;
        }
    }
    return false;
}
int main(int argc, char **argv) {

    vector<Pais>paises={
        {1,"Argentina",{"Chile", "Bolivia", "Paraguay", "Brasil", "Uruguay"},false},
    {2,"Bolivia",{"Peru", "Brasil", "Paraguay", "Argentina", "Chile"},false},
    {3,"Brasil",{"Uruguay", "Argentina", "Paraguay", "Bolivia", "Peru", "Colombia", "Venezuela", "Guyana", "Surinam"},false},
    {4,"Chile",{"Peru", "Bolivia", "Argentina"},false},
    {5,"Colombia", {"Venezuela", "Brasil", "Peru", "Ecuador"},false},
     {6,"Ecuador", {"Colombia", "Peru"},false},
    {7,"Guyana", {"Venezuela", "Brasil","Surinam"},false},
    {8,"Paraguay",{"Bolivia", "Brasil", "Argentina"},false},
    {9,"Peru", {"Ecuador", "Colombia", "Brasil", "Bolivia", "Chile"},false},
    {10,"Surinam", {"Guyana", "Brasil"},false},
    {11,"Uruguay", {"Brasil", "Argentina"},false},
    {12,"Venezuela", {"Colombia", "Brasil", "Guyana"},false}

    };

    vector<Color> colors(4);
    sort(paises.begin(), paises.end(), comparaVecinos);

    for (int i = 0; i < colors.size(); i++) {
        for (int j = 0; j < paises.size(); j++) {
            if (!paises[j].pintado and !esPaisExcluido(colors[i], paises[j].nombre)) {
                paises[j].pintado = true;
                colors[i].paises.push_back(paises[j].nombre);
                for (int k=0;k<paises[j].vecinos.size();k++) {
                    if (!esPaisExcluido(colors[i],paises[j].vecinos[k]))
                        colors[i].antis.push_back(paises[j].vecinos[k]);
                }
            }
        }
    }

    for (int i=0;i<colors.size();i++) {
        cout<<"Color "<<i+1<<":";
        for (int j=0;j<colors[i].paises.size();j++) {
            cout<<" "<<colors[i].paises[j];
        }
        cout<<endl;
    }





    return 0;
}
