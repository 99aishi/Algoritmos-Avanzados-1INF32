#include <iostream>
#include <vector>
#define alfa 0.25
#define x 0.6
#define y 0.4
#define P 150
using namespace std;
struct Proyecto {
    string nombre;
    int b;
    int c;
    string categoria;
    int r;
    double metrica;
};

void calcularMetrica(vector<Proyecto>& proyectos) {
    for (auto &p : proyectos) {
        p.metrica=x*(double(p.b)/p.c)+y*(10-p.r);
    }
}
void GRASP(vector<Proyecto>proyectos, int n) {

    vector<Proyecto> solu;
    srand(time(NULL));
    






}
int main() {
    vector<Proyecto> proyectos = {
        {"P0", 80, 40, "Marketing", 8},
        {"P1", 120, 90, "TI", 5},
        {"P2", 60, 20, "Producción", 3},
        {"P3", 30, 10, "Logística", 9},
        {"P4", 200, 150, "RRHH", 2},
        {"P5", 55, 25, "Marketing", 7},
        {"P6", 110, 70, "TI", 10},
        {"P7", 45, 30, "Producción", 4},
        {"P8", 75, 50, "Logística", 6},
        {"P9", 90, 60, "RRHH", 1}
    };
    calcularMetrica(proyectos);
    int n=proyectos.size();
    GRASP(proyectos,n);

    return 0;

}
