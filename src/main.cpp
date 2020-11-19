#include <iostream>
#include "connectivity.cpp"
#include "metrics.cpp"
#include "FluxConvectifs.cpp"

int main() {

    Connectivity();

    Metrics();

    //creation de n variables W (1 par element) contenant les variables_conservatrices
    struct variables_conservatrices {
      double rho;
      double rho_u;
      double rho_v;
      double rho_E;
    } W [NELEM];

    //initialisation des flux FluxConvectifs pour 1 elem, a faire pour chaque element quand on calcule les flux
    vector<double> valeursInit = {4,3,2,1,0};
    FluxConvectifs valeurs(valeursInit);
    cout << valeurs.rho << '\n';
    cout << valeurs.u << '\n';
    cout << valeurs.v << '\n';
    cout << valeurs.H << '\n';
    cout << valeurs.p << '\n';
    return 0;
}
