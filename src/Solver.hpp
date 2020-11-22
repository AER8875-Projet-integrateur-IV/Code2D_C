//

//
//  Created by charles D. on 2020-10-25.
//

#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>

struct variables_conservatrices {
  double rho;
  double rho_u;
  double rho_v;
  double rho_E;
};
void Solve();
double CalculateDeltat(double volume);
std::vector<double> CalculateFlux();
std::vector<double> CalculateW();
void UpdateW();
void UpdateGhostsCells();



#endif /* Solver_hpp */
