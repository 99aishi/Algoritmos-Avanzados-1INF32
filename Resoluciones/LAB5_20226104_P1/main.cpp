#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#define NUMIND 20
#define TSELECCION 0.30
#define TCASAMIENTO 0.50
#define ITERACIONES 1000

#define N 7
#define P 400
using namespace std;


struct Proyecto {
    int id;
    int costo;
    int ganancia;
    vector<int> predecesores;
};
int calcularFitness(vector<int>individuo, vector<Proyecto>proyectos,int n) {

    int fit=0;
    for (int i = 0; i < n; i++) {
        if (individuo[i]==1) {
            fit += proyectos[i].ganancia;
        }
    }

    return fit;
}
bool validarPredecesores(vector<int>individuo, vector<Proyecto>proyectos,int n) {

    for (int i = 0; i < n; i++) {
        if (individuo[i]==1 and !proyectos[i].predecesores.empty()) {
            for (int j = 0; j < proyectos[i].predecesores.size(); j++) {
                if (individuo[proyectos[i].predecesores[j]-1]!=1) {
                    return false;
                }
            }
        }
    }
    return true;

}

bool aberracion(vector<int>individuo, vector<Proyecto>proyectos,int n) {

    int costoTotal=0;
    for (int i=0;i<n;i++) {
        if (individuo[i] == 1) {
            costoTotal += proyectos[i].costo;
        }
    }
    if (costoTotal > P) return true;

    if (!validarPredecesores(individuo,proyectos,n)) return true;

    return false;
}
void generarPoblacion(vector<Proyecto> proyectos,vector<vector<int>>&poblacion,int n) {
    int cont =0;
    vector<int> cromosoma(n);
    srand(time(NULL));
    while (cont<NUMIND) {
        for (int i = 0; i < n; i++) {
            cromosoma[i] = rand() % 2;
        }
        if (!aberracion(cromosoma,proyectos,n)) {
            poblacion.push_back(cromosoma);
            cont++;
        }
    }
}
void mostrarPoblacion(vector<vector<int>> poblacion,vector<Proyecto>proyectos, int n) {
    for (int i = 0; i < poblacion.size(); i++) {
        for (int j = 0; j < poblacion[i].size(); j++) {
            cout << poblacion[i][j] << " ";
        }
        cout << " fo= " << calcularFitness(poblacion[i],proyectos,n);
        cout << endl;
    }
    cout << "------------------------------------------------------------";
    cout << endl;
}

void calculaSupervivencia(vector<int>& supervivencia,vector<vector<int>>poblacion, vector<Proyecto>proyectos,int n) {
    int sumaFit=0;
    for (int i=0;i<poblacion.size();i++) {
        sumaFit+= calcularFitness(poblacion[i],proyectos,n);
    }
    if (sumaFit==0) {
        supervivencia.assign(1,0);
        return;
    }
    for (int i=0;i<poblacion.size();i++) {
        int superFit= round(100*(double)calcularFitness(poblacion[i],proyectos,n)/sumaFit);
        supervivencia.push_back(superFit);
    }


}
void cargarRuleta(int *ruleta, vector<int> supervivencia) {
    int cont =0;
    for (int i=0 ; i<supervivencia.size(); i++) {
        for (int j=0 ; j<supervivencia[i]; j++) {
            ruleta[cont++]=i;
        }
    }
    while (cont < 100)
        ruleta[cont++]=-1;

}
void seleccion(vector<vector<int>>poblacion,vector<vector<int>>&padres, vector<Proyecto>proyectos,int n) {

    vector<int>supervivencia;
    int ruleta[110]{};
    calculaSupervivencia(supervivencia,poblacion,proyectos,n);
    cargarRuleta(ruleta,supervivencia);

    int npadres = round(poblacion.size()*TSELECCION);
    for (int i = 0; i<npadres; i++) {
        int ganador = rand()%100;
        int ind= ruleta[ganador];
         if (ind!=-1) {
            padres.push_back(poblacion[ind]);
         }
    }
}

void creaHijo(vector<int>padre, vector<int>madre,vector<int>&hijo) {

    int pos= round(padre.size()*TCASAMIENTO);
    for (int i=0;i<pos;i++)
        hijo.push_back(padre[i]);
    for (int i=pos;i<madre.size();i++)
        hijo.push_back(madre[i]);
}
void casamiento(vector<vector<int>> &poblacion,vector<vector<int>> padres, vector<Proyecto>proyectos,int n) {

    for (int i=0;i<padres.size();i++) {
        for (int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                creaHijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,proyectos,n)) {
                    poblacion.push_back(hijo);
                }
            }
        }
    }


}
int decimal(vector<int>cromo) {
    int num=0;
    for (int i=0;i<cromo.size();i++) {
        num+= cromo[i] * pow(2,i);
    }
    return num;
}
void mataclon(vector<vector<int>>&poblacion) {
    map<int,vector<int>> indunicos;

    for (int i=0;i<poblacion.size();i++) {
        int dec = decimal(poblacion[i]);
        indunicos[dec]= poblacion[i];
    }
    poblacion.clear();

    for (map<int,vector<int>>::iterator it=indunicos.begin();it!=indunicos.end();it++)
        poblacion.push_back(it->second);
}

void regeneracionPoblacion(vector<vector<int>> &poblacion, vector<Proyecto>proyectos,int n) {
    sort(poblacion.begin(),poblacion.end(),[proyectos,n](const vector<int>&a,const vector<int>&b) {
        int fita = calcularFitness(a,proyectos,n);
        int fitb = calcularFitness(b,proyectos,n);
        return fita>fitb;
    });
    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());
}
void mostrarMejor(vector<vector<int>> poblacion, vector<Proyecto> proyectos,int n) {
    int mejor=0;

    for (int i=1;i<poblacion.size();i++) {
        if (calcularFitness(poblacion[i],proyectos,n)>calcularFitness(poblacion[mejor],proyectos,n))
            mejor=i;
    }

    cout << "Proyectos Seleccionados: ";
    for (int i=0;i<n;i++) {
        if (poblacion[mejor][i]==1)
            cout << proyectos[i].id << " ";
    }
    cout << endl;
    cout << "Beneficio total :" << calcularFitness(poblacion[mejor],proyectos,n) << endl;




}
void algoritmoGenetico(vector<Proyecto> proyectos, int n) {
    vector<vector<int>> poblacion;
    generarPoblacion(proyectos,poblacion,n);
    cout << "Poblacion Inicial: "<< endl;
    mostrarPoblacion(poblacion,proyectos,n);

    for (int i = 0; i < ITERACIONES; i++) {
        vector<vector<int>> padres;
        seleccion(poblacion,padres,proyectos,n);
        casamiento(poblacion,padres,proyectos,n);
        mataclon(poblacion);
        regeneracionPoblacion(poblacion,proyectos,n);
    }
    mostrarPoblacion(poblacion,proyectos,n);
    mostrarMejor(poblacion,proyectos,n);
}


int main() {


    vector<Proyecto> proyectos;
    proyectos.push_back(Proyecto(1 ,100, 200));
    proyectos.push_back(Proyecto(2 ,50, 300,{1}));
    proyectos.push_back(Proyecto(3 ,150, 300,{1}));
    proyectos.push_back(Proyecto(4 ,50, 400));
    proyectos.push_back(Proyecto(5 ,50, 200,{4}));
    proyectos.push_back(Proyecto(6 ,150, 800,{2,4}));
    proyectos.push_back(Proyecto(7 ,100, 150));

    int n = proyectos.size();
    algoritmoGenetico(proyectos,n);

    return 0;






}
