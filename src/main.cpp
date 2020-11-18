#include <iostream>
#include "connectivity.cpp"
#include "metrics.cpp"
#include "VariablesConservatrices.cpp"

int main() {
    vector<double> valeursInit = {4,3,2,1,0};
    Connectivity();
    Metrics();
    VariablesConservatrices valeurs(valeursInit);
    cout << valeurs.rho << '\n';
    cout << valeurs.u << '\n';
    cout << valeurs.v << '\n';
    cout << valeurs.H << '\n';
    cout << valeurs.p << '\n';
    return 0;
}
