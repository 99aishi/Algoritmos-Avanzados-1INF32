//
// Created by 999 on 5/17/2026.
//

#ifndef E1_P1_25_2_REGION_H
#define E1_P1_25_2_REGION_H
#include <vector>

#include "celda.h"
using namespace std;
struct Region {
    vector<Celda> celdas;
    int cantCeldas;
    int suma;
};
#endif //E1_P1_25_2_REGION_H