#include <iostream>
#include "connectivity.cpp"
#include "metrics.cpp"
#include "FluxConvectifs.cpp"
#include "Solver.cpp"
#include "BoundaryConditions.cpp"
#include <list>
using namespace std;


int main() {

    Connectivity();

    Metrics();

    //creation de NELEM variables W (1 par element) contenant les variables_conservatrices
    variables_conservatrices W [nElemTot];
    //initialisation des valeurs conservatrices
    for (size_t iElem = 0; iElem < nElemTot; iElem++) {
      W[iElem].Energy = ComputeEnergy(rhoInf, uInf,vInf, PressInf,gammaFluid);
      W[iElem].rho = rhoInf;
      W[iElem].rho_u = rhoInf * uInf;
      W[iElem].rho_v = rhoInf * vInf;
      W[iElem].rho_E = rhoInf * W[iElem].Energy;
    }


    //initialisation des flux FluxConvectifs pour nelem, a faire pour chaque element quand on calcule les flux
    double HInf = ComputeEnthalpy(rhoInf, uInf, vInf, PressInf, gammaFluid);
    vector<double> valeursInit = {rhoInf,uInf,vInf,HInf,PressInf};

    FluxConvectifs valeurs[nElemTot];

    for (size_t i = 0; i < nElemTot; i++) {
      valeurs[i].initialize(valeursInit);

      // cout << valeurs[i].rho << '\n';
      // cout << valeurs[i].u << '\n';
      // cout << valeurs[i].v << '\n';
      // cout << valeurs[i].H << '\n';
      // cout << valeurs[i].p << '\n';
    }



    // vector<double> a = CalculateFlux(valeurs[0], valeurs[1], normalVec[0][0]);
    // cout << a[0] << '\n';

    //initia de l'ecoulement aux valeurs a l'infini

    //mise a jour des conditions fantomes (BoundaryConditions)

    //iteration sur les cellules jusqu'a convergence
    double erreur_max = 1000;
    double critere = 1.0e-5;
    double nbIterMax = 10;
    int Iter = 0;
    while (erreur_max > critere && Iter < nbIterMax) {
      cout << "test" << '\n';
      Solve(valeurs, W);
      Iter++;
      cout << "rho cell 0: "<< W[0].rho << " Iter: " << Iter << '\n';
    }



    return 0;
}
