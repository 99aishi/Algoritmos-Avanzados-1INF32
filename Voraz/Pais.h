//
// Created by 999 on 5/9/2026.
//

#ifndef VORAZ_PAIS_H
#define VORAZ_PAIS_H
#include <string>
#include <vector>
using namespace std;

struct Pais {
    int num;
    string nombre;
    vector<string> vecinos;
    bool pintado;
};
#endif //VORAZ_PAIS_H