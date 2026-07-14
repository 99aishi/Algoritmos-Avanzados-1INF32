#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#define NUMIND 20
#define ITERACIONES 50
#define TSELECCION 0.5
#define PCASAMIENTO 0.5
#define TMUTACION 0.3
using namespace std;

bool aberracion(vector<int> cromosoma,vector<int> maquinas,vector<vector<int>>performances, int n , int m) {

    vector<int> seleccionados(m+1);

    for (int i = 0; i<n ; i++) {
        seleccionados[cromosoma[i]]++;
    }
    if (seleccionados[0]>0) return true;
    for (int i = 1; i<seleccionados.size() ; i++) {
        if (seleccionados[i]>maquinas[i-1] or seleccionados[i]==0) return true;
    }
    return false;
}


void generaPoblacion(vector<int> maquinas,vector<vector<int>>performances, int n , int m,vector<vector<int>>&poblacion) {
    int cont=0;
    srand(time(NULL));

    while (cont < NUMIND) {
        int longitud=n;
        vector<int> cromosoma(longitud);
        for (int i = 0; i < longitud; i++) {
            cromosoma[i]=rand()%(m+1);
        }

        if (!aberracion(cromosoma,maquinas,performances,n,m)) {
            poblacion.push_back(cromosoma);
            cont++;
        }

    }




}

void muestraPoblacion(const vector<vector<int>> & poblacion,vector<vector<int>>performances) {
    for (int i = 0; i < poblacion.size(); i++) {
        int suma=0;
        for (int j = 0; j < poblacion[i].size(); j++) {
            cout << poblacion[i][j] << " ";
            suma+=performances[j][poblacion[i][j]-1];
        }
        cout << suma;
        cout << endl;
    }
}
int calculaFitness(vector<int>individuo,vector<vector<int>>performances) {
    int fit=0;
    for (int i = 0; i < individuo.size(); i++) {
        fit+=performances[i][individuo[i]-1];
    }
    return fit;

}
void calculaSupervivencia(vector<vector<int>>poblacion,vector<int>&supervivencia,vector<vector<int>>performances, int n , int m) {
    int sumaFit=0;
    for (int i=0; i<poblacion.size(); i++) {
        sumaFit+=calculaFitness(poblacion[i],performances);
    }
    for (int i=0;i<poblacion.size();i++) {
        int porc=round(100.0*calculaFitness(poblacion[i],performances)/sumaFit);
        supervivencia.push_back(porc);
    }
}


void cargaRuleta(const vector<int>supervivencia, vector<int>&ruleta) {
    int cont=0;
    for (int i = 0; i < supervivencia.size(); i++) {
        for (int j=0;j<supervivencia.size(); j++) {
            if (cont<100) {
                ruleta[cont++]=i;
            }
        }
    }
}

void seleccion(vector<vector<int>>&padres,vector<vector<int>>poblacion,vector<vector<int>>performances, int n , int m) {

    vector<int>ruleta(100,-1);
    vector<int>supervivencia;
    calculaSupervivencia(poblacion,supervivencia,performances,n,m);
    cargaRuleta(supervivencia,ruleta);
    int npadres= round(poblacion.size()*TSELECCION);
    for (int i=0;i<npadres;i++) {
        int ticket=rand()%100;
        if (ruleta[ticket]!=-1)
            padres.push_back(poblacion[ruleta[ticket]]);
    }

}
void creaHijo(vector<int>padre,vector<int>madre,vector<int>&hijo) {
    int pos = round(padre.size()*PCASAMIENTO);
    for (int i=0;i<pos;i++)
        hijo.push_back(padre[i]);
    for (int i=pos;i<madre.size();i++)
        hijo.push_back(madre[i]);
}
void casamiento(vector<vector<int>> & poblacion, const vector<vector<int>> & padres,
    vector<int> maquinas,vector<vector<int>>performances, int n , int m) {

    for (int i = 0; i < padres.size(); i++) {
        for (int j = 0; j < padres.size(); j++) {
            if (i!=j) {
                vector<int>hijo;
                creaHijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,maquinas,performances,n,m)) {
                    poblacion.push_back(hijo);
                }
            }
        }
    }

}

void mutacion(vector<vector<int>> & poblacion, vector<vector<int>> & padres, const vector<int> & maquinas, const vector<vector<int>> & performances, int n, int m){
    int nmut=round(n*TMUTACION);
    for(int i=0;i<padres.size();i++){
        int cont=0;
        while(cont<nmut){
            int gen=rand()%n;
            padres[i][gen]=rand()%(m+1);
            cont++;
        }

        if(!aberracion(padres[i],maquinas,performances,n,m))
            poblacion.push_back(padres[i]);
    }
}
void inversion(vector<vector<int>> & poblacion, vector<vector<int>> & padres, const vector<int> & maquinas, const vector<vector<int>> & performances, int n, int m) {
    for (int i = 0; i < padres.size(); i++) {
        for (int j=0;j<n;j++) {
            padres[i][j]=m-padres[i][j];
        }
        if (!aberracion(padres[i],maquinas,performances,n,m)) {
            poblacion.push_back(padres[i]);
        }
    }
}
string clave(vector<int> ind){
    string s;
    for(int i=0;i<ind.size();i++)
        s+=to_string(ind[i]);
    return s;
}
void mataclon(vector<vector<int>>&poblacion) {
    map<string,vector<int>> unicos;
    for(int i=0;i<poblacion.size();i++)
        unicos[clave(poblacion[i])]=poblacion[i];
    poblacion.clear();
    for(auto &ind:unicos)
        poblacion.push_back(ind.second);
}
void regenerapoblacion(vector<vector<int>> & poblacion,vector<vector<int>>performances) {
    mataclon(poblacion);
    sort(poblacion.begin(),poblacion.end(),[performances](vector<int>a,vector<int>b) {
            return calculaFitness(a,performances)>calculaFitness(b,performances);
    });

    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}
void muestramejor(vector<vector<int>> poblacion,vector<vector<int>>performances){
    int mejor=0;
    cout<<endl<<"Mejor fitness = " <<calculaFitness(poblacion[0],performances) <<endl;
    for(int ind:poblacion[0])
        cout<<ind<<" ";
    cout<<endl;

}
void asignacionOperadores(vector<int> maquinas,vector<vector<int>>performances, int n , int m) {

    vector<vector<int>>poblacion;
    generaPoblacion(maquinas,performances,n,m,poblacion);
    cout << "Poblacion inicial: "<<endl;
    muestraPoblacion(poblacion,performances);

    for (int it =0 ; it<ITERACIONES ; it++) {
        cout << " -------------------------------- "<< endl;
        vector<vector<int>>padres;
        seleccion(padres,poblacion,performances,n,m);
        casamiento(poblacion,padres,maquinas,performances,n,m);
        mutacion(poblacion,padres,maquinas,performances,n,m);
        inversion(poblacion,padres,maquinas,performances,n,m);
        regenerapoblacion(poblacion,performances);
        cout<<endl<<"ITERACION "<<it+1<<endl;
        muestramejor(poblacion,performances);
    }




}

int main() {

    int n =7, m =5;
    vector<vector<int>> performances={
        {3 ,4, 5, 3, 6},
        {3 ,2 ,1 ,2 ,5},
        {1 ,8 ,5 ,3 ,1},
        {3 ,6 ,4 ,6 ,3},
        {8, 2, 10, 4, 9},
        {7 ,6 ,3 ,2 ,3},
        {4 ,5 ,1 ,3 ,6}};

    vector<int>maquinas={2,3,1,4,3};

    asignacionOperadores(maquinas,performances,n,m);






    return 0;
}
