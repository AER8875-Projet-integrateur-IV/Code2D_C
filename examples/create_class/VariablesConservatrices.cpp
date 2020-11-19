//

//
//  Created by charles D. on 2020-10-25.
//


#include <iostream>
using namespace std;

class VariablesConservatrices {
  public:
    VariablesConservatrices(vector<double> valeursInit);

    double rho;
    double u;
    double v;
    double H;
    double p;

};

VariablesConservatrices::VariablesConservatrices(vector<double> valeursInit) {

  rho = valeursInit[0];
  u = valeursInit[1];
  v = valeursInit[2];
  H = valeursInit[3];
  p = valeursInit[4];

}
