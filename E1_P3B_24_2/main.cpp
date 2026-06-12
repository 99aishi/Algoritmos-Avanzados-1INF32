/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/18/2026, 12:20 PM
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include "Tarea.h"
using namespace std;

bool compara(const Tarea &a,const Tarea &b) {
    if (a.hora_max==b.hora_max) {
        return a.ganancia>b.ganancia;
    }
    return a.hora_max>b.hora_max;
}
bool comparaGanancia(const Tarea &a,const Tarea &b) {
    cout << a.hora_max << " " << b.hora_max<< endl;
    return a.ganancia>b.ganancia;
}
void ordenarRango(vector<Tarea>&tareas,int indIni,int indFinal) {

    sort(tareas.begin()+indIni,tareas.begin()+indFinal,comparaGanancia);

}
void ordenarMaxPermitido(vector<Tarea>&tareas,int hora) {
    int indIni=0;
    bool inicio=false;
    int indFinal=0;

    for (int i = 0; i < tareas.size(); i++) {
        if (hora<=tareas[i].hora_max and inicio==false) {
            indIni=i;
            inicio=true;
        }else {
            if (hora>tareas[i].hora_max and inicio==true) {
                indFinal=i-1;
                break;
            }
        }
    }
    if (hora==1) {
        indFinal=tareas.size();
    }

    ordenarRango(tareas,indIni,indFinal);
}
int main(int argc, char **argv) {

    vector<Tarea> tareas;
    tareas.push_back(Tarea {'A',2,100});
    tareas.push_back(Tarea {'B',1,19});
    tareas.push_back(Tarea {'C',2,27});
    tareas.push_back(Tarea {'D',1,120});
    tareas.push_back(Tarea {'E',3,15});

    // tareas.push_back(Tarea {'A',4,20});
    // tareas.push_back(Tarea {'B',1,40});
    // tareas.push_back(Tarea {'C',2,40});
    // tareas.push_back(Tarea {'D',2,30});

    sort(tareas.begin(), tareas.end(),compara);
    vector<Tarea>solu;
    int hora_final=tareas.begin()->hora_max;
    int hora_actual=hora_final;
    int i=0;
    while (hora_actual>0) {
        if (hora_actual<=tareas[0].hora_max) {
            solu.push_back(tareas[0]);
            tareas.erase(tareas.begin());
            sort(tareas.begin(), tareas.end(),compara);
        }
        hora_actual--;
        ordenarMaxPermitido(tareas,hora_actual);

    }// ordenar tdo lo que se puede cumplir en esa hora con la ganancia maxima
    for (Tarea t : solu) {
        cout<<t.id<<" ";
    }
    cout<<endl;
    return 0;
}