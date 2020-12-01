//

//
//  Created by charles D. on 2020-10-25.
//

#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>

double gammaFluid = 1.4;
double K = 8.3145;
double AOA = 0;
double Mach = 1.2;
double uInf = sqrt(1.4) * Mach * cos(AOA);
double vInf = sqrt(1.4) * Mach * sin(AOA);
double CFL = 0.7;
double TempInf = 1;
double PressInf = 1;
double rhoInf = 1;


struct variables_conservatrices {
  double rho;
  double rho_u;
  double rho_v;
  double rho_E;
  double pressure;
  double Enthalpy;
  double Energy;
};
void Solve();
double CalculateDeltat(int iElem, FluxConvectifs valeurs, double volume, vector<vector<double> > normal);
std::vector<double> CalculateFlux(FluxConvectifs left, FluxConvectifs right, vector<double> n);
std::vector<double> CalculateW(int iElem, double dt, double volume, vector<double> Fc);
void UpdateW(int iElem, variables_conservatrices* produits, vector<double> deltaW);
void UpdateGhostsCells();
double ComputeEnergy(double rho, double u, double v, double p, double gamma);


#endif /* Solver_hpp */
