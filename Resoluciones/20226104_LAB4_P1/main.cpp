#include <algorithm>
#include <iostream>
#include <vector>

#define ITERACIONES 1000
#define alfa 0.30

#define P 400
#define N 7

using namespace std;


struct Proyecto {
    int id;
    int costo;
    int ganancia;
    vector<int> predecesores;
};
int buscarInd(vector<Proyecto> vproyectos, double rcl) {
    int j=0;
    for (int i=0; i<vproyectos.size(); i++) {
        if (vproyectos[i].ganancia >= rcl) {
            j++;
        }
    }
    return j;
}
int buscarProyecto(vector<Proyecto> vproyectos, int id) {
    if (id <=0 ) {
        return 0;
    }

    for (int i=0; i<vproyectos.size(); i++) {
        if (vproyectos[i].id == id) {
            return i;
        }
    }
}
bool validarPredecesores(vector<Proyecto>arr, Proyecto proy) {

    if (proy.predecesores.size() == 0) {
        return true;
    }

    for (int i=0; i<proy.predecesores.size(); i++) {
        bool encontrado=false;
        for (int j=0; j<arr.size(); j++) {
            if (proy.predecesores[i] == arr[j].id) {
                encontrado=true;
            }
        }
        if (!encontrado)
            return false;
    }
    return true;

}
bool compara(Proyecto a, Proyecto b) {

    return a.ganancia > b.ganancia;
}
bool validarAgregados(Proyecto proyecto, vector<Proyecto> vsolucion) {
    for (int i=0; i<vsolucion.size(); i++) {
        if (vsolucion[i].id == proyecto.id) {
            return true;
        }
    }
    return false;
}
vector<Proyecto> proyectosDisponibles(vector<Proyecto> vproyectos,vector<Proyecto> solucion) {
    vector<Proyecto> validos;
    for (int i=0; i<vproyectos.size(); i++) {
        if (validarAgregados(vproyectos[i], solucion)) {
            continue;
        }
        if (vproyectos[i].predecesores.size() == 0) {
            validos.push_back(vproyectos[i]);
        }else {
            if (validarPredecesores(solucion, vproyectos[i])) {
                validos.push_back(vproyectos[i]);
            }
        }
    }

    return validos;


}
void grasp(vector<Proyecto> proyectos, int n) {
    int mayorGanancia = INT_MIN;
    vector<Proyecto>mejorSolu;
    srand(time(NULL));
    for (int i =0;i<ITERACIONES;i++) {

        vector<Proyecto> vproyectos;
        vproyectos.insert(vproyectos.begin(), proyectos.begin(), proyectos.begin()+n);
        sort(vproyectos.begin(), vproyectos.end(),compara);
        int acumulacionCosto = 0;
        int acumulacionGanancia = 0;
        vector<Proyecto>solucion;
        vector<Proyecto> validos = proyectosDisponibles(vproyectos,solucion);
        while (!vproyectos.empty()) {

            validos = proyectosDisponibles(vproyectos,solucion);
            if (validos.empty()) break;
            int beta = validos[0].ganancia;
            int tau= validos[validos.size()-1].ganancia;
            double rcl = beta - alfa*(beta-tau);

            int ind=buscarInd(validos,rcl);

            int index = rand()%ind;
            int indVPro= buscarProyecto(vproyectos,validos[index].id);
            if (solucion.size()>=N) break;
            if (acumulacionCosto + validos[index].costo < P) {
                    solucion.push_back(validos[index]);
                    acumulacionGanancia+=validos[index].ganancia;
                    acumulacionCosto+=validos[index].costo;
            }
            vproyectos.erase(vproyectos.begin()+indVPro);
        }

        if (mayorGanancia < acumulacionGanancia) {
            mayorGanancia = acumulacionGanancia;
            mejorSolu = solucion;
        }
    }
    cout << "Proyectos : ";
    for (int i=0; i<mejorSolu.size(); i++) {
        cout << mejorSolu[i].id << " ";
    }
    cout << endl<< "Beneficio: " << mayorGanancia << endl;

}


int main() {

    vector<Proyecto> proyectos;
    proyectos.push_back(Proyecto(1,100,200));
    proyectos.push_back(Proyecto(2,50,300,{1}));
    proyectos.push_back(Proyecto(3,150,300,{1}));
    proyectos.push_back(Proyecto(4,50,400));
    proyectos.push_back(Proyecto(5,50,200,{4}));
    proyectos.push_back(Proyecto(6,150,800,{2,4}));
    proyectos.push_back(Proyecto(7,100,250));
    int n = proyectos.size();
    grasp(proyectos, n);




    return 0;
}