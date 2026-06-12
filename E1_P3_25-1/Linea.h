//
// Created by 999 on 5/17/2026.
//

#ifndef E1_P3_25_1_LINEA_H
#define E1_P3_25_1_LINEA_H
#include "Orden.h"
using namespace std;
struct Linea {
    int id;
    int tiempoAcumulado;
    vector<Orden> ordenes;

};
#endif //E1_P3_25_1_LINEA_H