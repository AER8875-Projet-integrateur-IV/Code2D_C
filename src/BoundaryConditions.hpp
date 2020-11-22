
//created by charles D. 22/11/2020



#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>
#include "Solver.hpp"



std::vector<double> WallCond(FluxConvectifs Bcell, FluxConvectifs cell2, vector<double> n);
std::vector<double> SuperInflow(variables_conservatrices W_a);
std::vector<double> SuperOutflow(variables_conservatrices W_d);
std::vector<double> SubInflow(FluxConvectifs a, FluxConvectifs d, std::vector<double> n, double c0, double rho0);
std::vector<double> SubOutflow(FluxConvectifs a, FluxConvectifs d, std::vector<double> n, double c0, double rho0;


#endif /* Solver_hpp */
