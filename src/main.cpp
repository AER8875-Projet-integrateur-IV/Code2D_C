#include <iostream>
#include "connectivity.cpp"
#include "metrics.cpp"
#include "FluxConvectifs.cpp"
#include "Solver.cpp"
#include <list>
using namespace std;

int main() {

    Connectivity();

    Metrics();

    //creation de NELEM variables W (1 par element) contenant les variables_conservatrices
    struct variables_conservatrices {
      double rho;
      double rho_u;
      double rho_v;
      double rho_E;
    } W [NELEM];

    //initialisation des flux FluxConvectifs pour nelem, a faire pour chaque element quand on calcule les flux
    vector<double> valeursInit = {4,3,2,1,0};
    FluxConvectifs valeurs[NELEM];

    for (size_t i = 0; i < NELEM; i++) {
      valeurs[i].initialize(valeursInit);

      // cout << valeurs[i].rho << '\n';
      // cout << valeurs[i].u << '\n';
      // cout << valeurs[i].v << '\n';
      // cout << valeurs[i].H << '\n';
      // cout << valeurs[i].p << '\n';
    }


    vector<double> a = CalculateFlux(valeurs[0], valeurs[1], normalVec[0][0]);
    cout << a[0] << '\n';

    return 0;
}
