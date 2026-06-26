#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <algorithm>
#define ITERACIONES 10
#define NUMIND 20
#define TSELECCION 0.5
#define PCASAMIENTO 0.5
#define TMUTACION 0.3

using namespace  std;

int calculafitness(vector<int>cromo,int *paq) {
    int sumap=0;
    for(int i=0;i<cromo.size();i++) {
        sumap+=cromo[i]*paq[i];
    }
    return sumap;
}

bool aberracion(vector<int>ind,int peso,int *paq) {
    return calculafitness(ind,paq)>peso;
}

void muestrapoblacion(vector<vector<int>>poblacion,int *paq) {
    for(int i=0;i<poblacion.size();i++) {
        for(int j=0;j<poblacion[i].size();j++) {
            cout<<poblacion[i][j]<<" ";
        }
        cout<<" fo="<< calculafitness(poblacion[i],paq)<<endl;
    }

}

int decimal(vector<int>ind) {
    int num=0;
    for (int i=0;i<ind.size();i++)
        num+=ind[i]*pow(2,i);
    return num;
}

void mataclon(vector<vector<int>> &poblacion) {
    map<int,vector<int>> indunicos;

    for(int i=0;i<poblacion.size();i++) {
        int dec=decimal(poblacion[i]);
        indunicos[dec]=poblacion[i];
    }
    poblacion.clear();
    for(map<int,vector<int>>::iterator it=indunicos.begin();
        it!=indunicos.end();it++) {
        poblacion.push_back(it->second);
    }
}


void generapoblacion(vector<vector<int>> &poblacion,
    int *paq,int n,int peso) {
    int cont=0;
    srand(time(NULL));
    while (cont<NUMIND) {
        vector<int> individuo;
        // aqui también cambiar
        for (int i=0; i<n; i++)
            individuo.push_back(rand()%2);
        if (!aberracion(individuo,peso,paq)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}

void calculasupervivencia(vector<vector<int>>poblacion,
    vector<int> &supervivencia,int *paq,int n,int peso) {
    int sumafitness=0;

    for(int i=0;i<poblacion.size();i++)
        sumafitness+=calculafitness(poblacion[i],paq);
    for (int i=0;i<poblacion.size();i++) {
        int superfit;
        superfit=round(100*(double)calculafitness(poblacion[i],paq)/sumafitness);
        supervivencia.push_back(superfit);
    }
}
void cargaruleta(vector<int >supervivencia,int *ruleta) {
    int cont=0;
    for (int i=0;i<supervivencia.size();i++)
        for (int j=0;j<supervivencia[i];j++) {
            ruleta[cont++]=i;
        }
}



void seleccion(vector<vector<int>>poblacion,vector<vector<int>> &padres,
   int *paq,int n,int peso ) {
    int ruleta[100]{-1};
    vector<int>supervivencia;
    calculasupervivencia(poblacion,supervivencia,paq,n,peso);
    cargaruleta(supervivencia,ruleta);
    int npadres=round(poblacion.size()*TSELECCION);

    for (int i=0;i<npadres;i++) {
        int ticket=rand()%100;
        if (ruleta[ticket]!=-1)
            padres.push_back(poblacion[ruleta[ticket]]);
    }
    //muestrapoblacion(padres,paq);
}

void creahijo(vector<int>padre,vector<int>madre,
    vector<int>&hijo) {
    int posi=round(padre.size()*PCASAMIENTO);

    for (int i=0;i<posi;i++)
        hijo.push_back(padre[i]);
    for (int i=posi;i<madre.size();i++)
        hijo.push_back(madre[i]);
}

void casamiento(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    int *paq, int peso) {

    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                creahijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,peso,paq))
                    poblacion.push_back(hijo);
            }


        }
}


void regenerapoblacion(vector<vector<int>> &poblacion,
        int *paq,int peso){
    mataclon(poblacion);
    // se evita emplear variables globales
    sort(poblacion.begin(), poblacion.end(),
         [paq](const vector<int>& a, const vector<int>& b) {
            int suma=0, sumb=0;
            for(int i=0;i<a.size();i++)
                suma += calculafitness(a, paq);
            for(int i=0;i<b.size();i++)
                sumb += calculafitness(b, paq);

            return suma > sumb;
         });
    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    int *paq, int peso) {
    int nmutaciones=round(padres[0].size()*TMUTACION);
    for (int i=0;i<padres.size();i++) {
        int cont=0;
        while (cont<nmutaciones) {
            int gen=rand()%padres[i].size();
            //aqui hay que cambiar si son numeros enteros
            if (padres[i][gen]==0) padres[i][gen]=1;
            else padres[i][gen]=0;
            cont++;
        }
        if (!aberracion(padres[i],peso,paq))
            poblacion.push_back(padres[i]);
    }
}

void inversion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    int *paq, int peso) {

    for (int i=0;i<padres.size();i++) {
        for (int j=0;j<padres[i].size();j++) {
            //cambiar para numeros enteros
            if (padres[i][j]==0) padres[i][j]=1;
            else padres[i][j]=0;
        }
        if (!aberracion(padres[i],peso,paq))
            poblacion.push_back(padres[i]);
    }
}

int muestramejor(vector<vector<int>> poblacion,
        int *paq,int peso){
    int mejor=0;
    // puede mejorarse ya que esta muestra mejor lo busca en toda la poblaciÃ³n
    // desde luego se puede asumir que es la posiciÃ³n 0 para nuestro caso
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[mejor],paq)<calculafitness(poblacion[i],paq))
            mejor=i;

    cout << endl<<"La mejor solucion es:" << calculafitness(poblacion[mejor],paq)<<endl;
    for(int i=0;i<poblacion[mejor].size();i++)
        cout << poblacion[mejor][i] << "  ";

    cout << endl;
    return peso-calculafitness(poblacion[mejor],paq);
}

void mochilaAG(int *paq,int n,int peso) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion,paq,n,peso);

    for(int i=0;i<ITERACIONES;i++) {
        vector<vector<int>>padres;
        muestrapoblacion(poblacion,paq);
        cout << endl;
        seleccion(poblacion,padres,paq,n,peso);
        casamiento(poblacion,padres,paq,peso);
        mataclon(poblacion);
        mutacion((poblacion),padres,paq,peso);
        inversion(poblacion,padres,paq,peso);

        regenerapoblacion(poblacion,paq,peso);
        muestrapoblacion(poblacion,paq);
        muestramejor(poblacion,paq,peso);

    }
}

int main() {
    int paq[]={1,2,2,12,4};
    int n=sizeof(paq)/sizeof(paq[0]);
    int peso=15;

    mochilaAG(paq,n,peso);

    return 0;
}