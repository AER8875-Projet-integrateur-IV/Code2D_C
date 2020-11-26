//

//
//  Created by charles D. on 2020-10-25.
//

#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>

double tolerance = 0.001;
double nbIterMax = 1000;
double gammaFluid = 1.4;
double K = 8.3145;
double Mach = 1.2;
double CFL = 0.7;
double TempInf = 1;
double PressInf = 1;
double AOA = 0;

struct variables_conservatrices {
  double rho;
  double rho_u;
  double rho_v;
  double rho_E;
};
void Solve();
std::vector<double> CalculateDT(double volume);
std::vector<double> CalculateFlux();
std::vector<double> CalculateW();
void UpdateW();
void UpdateGhostsCells();



#endif /* Solver_hpp */
