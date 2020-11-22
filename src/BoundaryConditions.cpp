
//created by CD

#include "BoundaryConditions.hpp"



std::vector<double> WallCond(FluxConvectifs Bcell, FluxConvectifs cell2, vector<double> n){
  std::vector<double> flux = {0, n[0]*cell2.p, n[1]*cell2.p, 0}; //p wall = p2, a verifier
  //faut-il set rho et p dans Bcell?
  return flux;

}

std::vector<double> SuperInflow(variables_conservatrices W_a){
  std::vector<double> W_b = {0,0,0,0};
  W_b[0] = W_a.rho; //pas sur que ca fonctionne
  W_b[1] = W_a.rho_u;
  W_b[2] = W_a.rho_v;
  W_b[3] = W_a.rho_E;

  return W_b;
}

std::vector<double> SuperOutflow(variables_conservatrices W_d){
  std::vector<double> W_b = {0,0,0,0};
  W_b[0] = W_d.rho; //pas sur que ca fonctionne
  W_b[1] = W_d.rho_u;
  W_b[2] = W_d.rho_v;
  W_b[3] = W_d.rho_E;

  return W_b;
}

std::vector<double> SubInflow(FluxConvectifs a, FluxConvectifs d, std::vector<double> n, double c0, double rho0){
  std::vector<double> v = {0,0,0,0};;
  v[0] = 0.5*(a.p + d.p - rho0*c0*(n[0]*(a.u-d.u)+n[1]*(a.v-d.v))); //p_b
  v[1] = a.rho + (v[0]-a.p)/pow(c0,2); //rho_b
  v[2] = a.u - n[0]*(a.p - v[0])/rho0/c0; //u_b
  v[3] = a.v + n[1]*(a.p - v[0])/rho0/c0; //v_b

  //where ρ0 and c0 represent a reference state. The reference state is normally set equal to
  //the state at the interior point (point d in Fig. 8.6). The values in point a are determined
  //from the free-stream state.

  return v;

}

std::vector<double> SubOutflow(FluxConvectifs a, FluxConvectifs d, std::vector<double> n, double c0, double rho0){
  std::vector<double> v = {0,0,0,0};;
  v[0] = a.p; //p_b
  v[1] = d.rho + (v[0] - d.p)/(pow(c0,2)); // rho_b
  v[2] = d.u + n[0]*(d.p - v[0])/rho0/c0; //u_b
  v[3] = d.v + n[1]*(d.p - v[0])/rho0/c0; //v_b

  //with p a being the prescribed static pressure.
  //Physical properties in the dummy cells/nodes can be obtained by linear extrapolation
  //from the states b and d.

  return v;
}
