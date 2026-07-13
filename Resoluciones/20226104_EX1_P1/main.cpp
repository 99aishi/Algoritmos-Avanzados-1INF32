#include <iostream>
#include <vector>
using namespace std;
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
#define D  60
#define W  15

void backtracking(vector<vector<int>> dist,vector<bool>esVisitado,vector<int> w,vector<int>v,int x,int y,
    int& acumulado,int &pesoAcum, int& impAcum,vector<int>&ruta, int& maxImp,vector<int>&Optima) {

    if (acumulado>D) {
        return;
    }
    if (x==0 and esVisitado[x]==true) {
        if (pesoAcum>W) {
            return ;
        }
        if (impAcum>maxImp) {
            Optima=ruta;
            maxImp=impAcum;
        }
        return ;
    }
    for (int i=0;i<esVisitado.size();i++) {
        if (i==x or esVisitado[i]==true) {
            continue;
        }
        acumulado+=dist[x][i];
        pesoAcum+=w[i];
        impAcum+=v[i];
        esVisitado[i]=true;
        ruta.push_back(i);
        backtracking(dist,esVisitado,w,v,i,y,acumulado,pesoAcum,impAcum,ruta,maxImp,Optima);
        acumulado-=dist[x][i];
        pesoAcum-=w[i];
        impAcum-=v[i];
        esVisitado[i]=false;
        ruta.pop_back();
    }
    return ;

}

int main() {

    vector<int> w = {0, 5, 10, 4};
    vector<int>v = {0, 25, 40, 15};
    vector<bool>esVisitado(w.size(),false);
    vector<vector<int>> dist = {{0, 10, 15, 20},
                                {10, 0, 35, 25},
                                {35, 35, 0, 30},
                                {20, 25, 30, 0}};
    int acumulado = 0;
    int maxImp = 0;
    int impAcum = 0;
    int pesoAcum = 0;
    // int acumulado = 0;
    vector<int>ruta;
    vector<int>optima;
    ruta.push_back(0);
    backtracking(dist, esVisitado, w,v,0,0,acumulado,pesoAcum,impAcum,ruta,maxImp,optima);



    cout << "Máxima importancia obtenida: ";
    cout << maxImp;
    cout<<endl;
    cout << "Ruta óptima seguida: ";
    for (int i=0;i<optima.size();i++) {
        cout<<optima[i];
        if (i==optima.size()-1)
            cout << " ";
        else
            cout << "->";
    }
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}