#include <algorithm>
#include <iostream>
#include <vector>
#define  K 50
#define alfa 0.20
#define ITERACIONES 1000
using namespace std;


struct Pedido{
    char nombre;
    int peso;
};

struct Camion {
    int acumulado;
    vector<Pedido> pedidos;
};
bool compara(Pedido p1, Pedido p2) {
    return p1.peso > p2.peso;
}
int buscarInd(vector<Pedido> pedidos, double rcl) {
    int i =0;
    for (auto p: pedidos) {
        if (p.peso>=rcl)
            i++;
    }
    return i;
}
void grasp(vector<Pedido> pedidos, int n) {
    int cantCam = INT_MAX;
    srand(time(NULL));
    vector<Camion>mejorSolu ;

    for (int i=0;i<ITERACIONES;i++) {
        vector<Pedido> vpedidos;

        vector<Camion>camiones;
        vpedidos.insert(vpedidos.begin(),pedidos.begin(),pedidos.begin()+n);
        sort(vpedidos.begin(),vpedidos.end(),compara);
        while (!vpedidos.empty()) {
            int beta=vpedidos[0].peso;
            int tau=vpedidos[vpedidos.size()-1].peso;
            double rcl=beta - alfa*(beta-tau);
            int ind = buscarInd(vpedidos,rcl);
            int indv= rand()%ind;
            bool agregado=false;
            int j=0;
            for (j=0;j<camiones.size();j++) {
                if (camiones[j].acumulado+vpedidos[indv].peso<=K) {
                    camiones[j].acumulado+=vpedidos[indv].peso;
                    camiones[j].pedidos.push_back(vpedidos[indv]);
                    agregado=true;
                    break;
                }
            }
            if (!agregado) {
                Camion cam;
                cam.pedidos.push_back(vpedidos[indv]);
                cam.acumulado=vpedidos[indv].peso;
                camiones.push_back(cam);
            }
            vpedidos.erase(vpedidos.begin()+indv);
        }

        if (cantCam>camiones.size()) {
            cantCam=camiones.size();
            mejorSolu=camiones;
        }

    }

    for (int i=0; i<mejorSolu.size(); i++) {
        cout << "Camion " << i+1 <<" Capacidad = " << K << "kg: ";
        for (int j=0; j<mejorSolu[i].pedidos.size(); j++) {
            cout << mejorSolu[i].pedidos[j].nombre << " ";
        }
        cout << "| Peso: "<< mejorSolu[i].acumulado <<"/"<<K<<"kg"<< endl;
    }


}


int main() {

    vector<Pedido> pedidos;
    pedidos.push_back(Pedido('A', 15));
    pedidos.push_back(Pedido('B', 32));
    pedidos.push_back(Pedido('C', 20));
    pedidos.push_back(Pedido('D', 42));
    pedidos.push_back(Pedido('E', 8));
    pedidos.push_back(Pedido('F', 18));

    int n = pedidos.size();
    grasp(pedidos,n);




    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}