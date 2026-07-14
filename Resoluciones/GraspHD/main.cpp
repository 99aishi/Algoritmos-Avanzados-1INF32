#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>
#define ITERACIONES 100000
#define alfa 0.3

using namespace std;

struct Sobjeto {
    int codigo;
    int velocidad;
    int capacidad;
};
struct Ssolucion {
    int disco;
    int tabla;
};

bool compara(Sobjeto a,Sobjeto b) {
    return a.velocidad > b.velocidad;
}

int buscaind(vector<Sobjeto>vobjeto,double maxrcl) {
    int cont=0;
    for (int i=0;i<vobjeto.size();i++)
        if (vobjeto[i].velocidad>=maxrcl)
            cont++;
    return cont;
}

void cargadisco(Sobjeto *tabla, Sobjeto *disco,int n,int m) {
    int mejor=INT_MIN;
    vector<Ssolucion>smaxima;
    srand(time(NULL));
    for(int i=0;i<ITERACIONES;i++) {
        vector<Ssolucion>resultado;
        vector<Sobjeto> vtabla;
        vector<Sobjeto> vdisco;
        int residual;

        vtabla.insert(vtabla.begin(),tabla,tabla+n);
        vdisco.insert(vdisco.begin(),disco,disco+m);
        sort(vtabla.begin(),vtabla.end(),compara);

        while (!vtabla.empty()) {
            // Primero tablas
            int betat=vtabla[0].velocidad;
            int taut=vtabla[vtabla.size()-1].velocidad;
            double rclt= betat-alfa*(betat-taut);
            int indt=buscaind(vtabla,rclt);
            int indat=rand()%indt;

            //Ahora discos
            sort(vdisco.begin(),vdisco.end(),compara);
            int betad=vdisco[0].velocidad;
            int taud=vdisco[vdisco.size()-1].velocidad;
            double rcld=betad-alfa*(betad-taud);
            int indd=buscaind(vdisco,rcld);
            int indad=rand()%indd;

            if (vdisco[indad].velocidad>=vtabla[indat].velocidad) {
                resultado.push_back({vdisco[indad].codigo,vtabla[indat].codigo});
                vdisco[indad].velocidad-=vtabla[indat].velocidad;
            }
            vtabla.erase(vtabla.begin()+indat);
        }
        sort(vdisco.begin(),vdisco.end(),compara);
        residual=vdisco[vdisco.size()-1].velocidad;
        if (residual>mejor) {
            mejor=residual;
            smaxima.clear();
            smaxima.insert(smaxima.begin(),resultado.begin(),resultado.end());
        }
    }
    for (int i=0;i<smaxima.size();i++)
        cout << smaxima[i].disco <<" "<<smaxima[i].tabla<<endl;
    cout << endl << mejor << endl;
}



int main() {
    Sobjeto tabla[]={
        {1,150,20},
        {2,100,10},
        {3,100,15},
        {4,50,100},
        {5,120,50},
        {6,60,100}
    };
    Sobjeto disco[]={
        {1,250,800},
        {2,200,750},
        {3,200,850}
    };
    int n=sizeof(tabla)/sizeof(tabla[0]);
    int m=sizeof(disco)/sizeof(disco[0]);

    cargadisco(tabla,disco,n,m);


    return 0;
}