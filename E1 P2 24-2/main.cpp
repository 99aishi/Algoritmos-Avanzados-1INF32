/* 
 * File:   ${NAME}.${EXTENSION}
 * Author: ${user}
 *
 * Created on 5/17/2026, 11:33 PM
 */

#include <iostream>
using namespace std;

void backtracking(int n , string &cadena,int pos, int &ape, int &cie) {
    if (ape<cie) {
        return;
    }
    if (ape+cie==n*2) {
        cout<<cadena<<endl;
        return;
    }

    if (ape<n){
        cadena+="(";
        ape++;
        backtracking(n,cadena,pos+1,ape,cie);
        if (cadena[cadena.size()-1]=='(')
            ape--;
        else
            cie--;
        cadena.pop_back();
        // cadena+=")";
        // cie++;
        // ape--;

        // return;
        // backtracking(n,cadena,pos+1,ape,cie);
    }
    cadena+=")";
    cie++;
    backtracking(n,cadena,pos+1,ape,cie);
    cadena.pop_back();
    cie--;
    // cadena+=")";
    // cie++;
}



int main(int argc, char **argv) {
    int n=3;

    string cadena="";
    int ape=0;
    int cie=0;
    backtracking(n,cadena,0,ape,cie);

    return 0;
}