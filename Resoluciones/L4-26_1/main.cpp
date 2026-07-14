#include <algorithm>
#include <iostream>
#include <vector>
#define ITERACIONES 1000
#define alfa 0.2
#define K 50
using namespace std;

struct Pedido {
    char cod;
    int peso;
};
struct Camion {
    int id;
    int acumulado;
    vector<Pedido> pedidos;
};

bool compara(Pedido p1, Pedido p2) {
    return p1.peso > p2.peso;
}
bool comparaCamion(Camion p1, Camion p2) {
    return p1.acumulado < p2.acumulado;
}


int buscarInd(const vector<Pedido> & vpedidos, double rcl) {
    int i=0;
    for (auto p : vpedidos) {
        if (p.peso>=rcl)
            i++;
    }
    return i;
}
int buscarIndCamion(const vector<Camion> & camiones, double rcl) {
    int i=0;
    for (auto c : camiones) {
        if (c.acumulado<=rcl)
            i++;
    }
    return i;
}
void buscaPosibles(vector<Camion>camiones, vector<Camion>  & posibles, Pedido p) {

    if (camiones.empty()) return;
    for (auto c : camiones) {
        if (p.peso+c.acumulado<K)
            posibles.push_back(c);
    }

}
void grasp(vector<Pedido> pedidos, int n) {
    int cantCam=INT_MAX;
    srand(time(NULL));
    vector<Camion> mejorSolu;


    for (int i = 0; i < ITERACIONES; i++) {
        vector<Pedido> vpedidos;
        vector<Camion> camiones;
        vpedidos.insert(vpedidos.begin(),pedidos.begin(),pedidos.begin()+n);
        sort(vpedidos.begin(),vpedidos.end(),compara);
        int cams=0;
        while (!vpedidos.empty()) {
            int beta=vpedidos[0].peso;
            int tau=vpedidos[pedidos.size()-1].peso;
            double minrcl= beta+alfa*(tau-beta);
            int indMin=buscarInd(vpedidos,minrcl);
            int indale=rand()%indMin;
            vector<Camion>posibles;
            buscaPosibles(camiones,posibles,vpedidos[indale]);
            if (posibles.empty()) {
                Camion c;
                c.acumulado=0;
                c.pedidos.push_back(vpedidos[indale]);
                c.acumulado+=vpedidos[indale].peso;
                c.id=cams+1;
                cams++;
                camiones.push_back(c);
            }else {
                sort(posibles.begin(),posibles.end(),comparaCamion);
                int beta_camion= posibles[0].acumulado;
                int tau_camion= posibles[posibles.size()-1].acumulado;
                double minRCL_camion= beta_camion+alfa*(tau_camion-beta_camion);
                int indMin_camion=buscarIndCamion(posibles,minRCL_camion);
                int indale_camion=rand()%indMin_camion;
                camiones[posibles[indale_camion].id-1].acumulado+=vpedidos[indale].peso;
                camiones[posibles[indale_camion].id-1].pedidos.push_back(vpedidos[indale]);
            }
            vpedidos.erase(vpedidos.begin()+indale);

        }
        if (cantCam>camiones.size()) {
            cantCam=camiones.size();
            mejorSolu=camiones;
        }

    }
    for (int i=0; i<mejorSolu.size(); i++) {
        cout << "Camion " << i+1 <<" Capacidad = " << K << "kg: ";
        for (int j=0; j<mejorSolu[i].pedidos.size(); j++) {
            cout << mejorSolu[i].pedidos[j].cod << " ";
        }
        cout << "| Peso: "<< mejorSolu[i].acumulado <<"/"<<K<<"kg"<< endl;
    }



}



int main() {

    Pedido p('A',15);
    Pedido p1('B',32);
    Pedido p2('C',20);
    Pedido p3('D',42);
    Pedido p4('E',8);
    Pedido p5('F',18);
    vector<Pedido> pedidos;
    pedidos.push_back(p);
    pedidos.push_back(p1);
    pedidos.push_back(p2);
    pedidos.push_back(p3);
    pedidos.push_back(p4);
    pedidos.push_back(p5);

    int n = pedidos.size();
    grasp(pedidos,n);

    return 0;
}