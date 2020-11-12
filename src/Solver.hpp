//

//
//  Created by charles D. on 2020-10-25.
//

#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>


double CalculateDeltat(double volume);
double CalculateResidual();
std::vector<double> CalculateW();
void UpdateW();
void UpdateGhostsCells();


#endif /* Solver_hpp */
