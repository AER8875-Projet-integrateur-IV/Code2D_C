//
//  connectivity.cpp
//  TestMesh
//
//  Created by Charles Dansereau on 2020-10-25.
//
#include "Solver.hpp"
using namespace std;

void Solve(){
  // iteration sur toutes les faces
  // appel des fonctions definies plus bas pour resoudre
}

std::vector<double> CalculateDeltat(double volume){
  vector<double> dt[NELEM]; // = {0};
  vector<double> c[NELEM]; //  = {0};
  double SSx = 0;
  double SSy = 0;
  double RayonSpecX;
  double RayonSpecY;
  double RayonSpec;

  for (int iElem = 0; iElem < NELEM; iElem++){
    for (int iFace = 0; iFace < cell2nodeStart[iElem]; iFace++){
      SSx += normalVec[iElem][iFace][0];
      SSy += normalVec[iElem][iFace][1];
      c[iElem] = sqrt(gammaFluid*FluxConvectifs[iElem].p/FluxConvectifs[iElem].rho);
      }
    RayonSpecX = 0.5*(abs(FluxConvectifs.u[iElem])+c)*SSx;
    RayonSpecY = 0.5*(abs(FluxConvectifs.v[iElem])+c)*SSy;
    RayonSpec = RayonSpecX+RayonSpecY;
    dt[iElem] = CFL*area[iElem]/RayonSpec;
  }
  return dt;
}

vector<double> CalculateFlux(FluxConvectifs left, FluxConvectifs right, vector<double> n){
  //calcul du r/sidu ici c'est ici que va le roe scheme
  double gamma = 1.4;

  double rho = sqrt(left.rho * right.rho);
  double u = (left.u * sqrt(left.rho) + right.u * sqrt(right.rho))/(sqrt(left.rho)+sqrt(right.rho));
  double v = (left.v * sqrt(left.rho) + right.v * sqrt(right.rho))/(sqrt(left.rho)+sqrt(right.rho));
  double H = (left.H * sqrt(left.rho) + right.H * sqrt(right.rho))/(sqrt(left.rho)+sqrt(right.rho));
  double q = sqrt(pow(u,2)+pow(v,2));
  double V = u*n[0] + v*n[1];
  double c = sqrt((gamma-1)*(H-pow(q,2)/2));
  //je fais *-1 a la mormale pour obtenir la normale de l<autre face
  double V_left = left.u*n[0] + left.v*n[1];
  double V_right = right.u*(-1)*n[0] + right.v*(-1)*n[1];
  double deltaV = V_left - V_right;
  double deltap = left.p - right.p;
  double deltarho = left.rho - right.rho;
  double deltau = left.u - right.u;
  double deltav = left.v - right.v;

  vector<double> deltaF1 = {1, u-c*n[0], v-c*n[0], H - c*V};
  for (size_t i = 0; i < 4; i++) {
    deltaF1[i] = deltaF1[i]*abs(V-c)*(deltap - rho*c*deltaV)/(2*pow(c,2));
  }

  vector<double> deltaF234 = {1, u, v, pow(q,2)/2};
  for (size_t i = 0; i < 4; i++) {
    deltaF234[i] = deltaF234[i]*abs(V)*(deltarho - deltap/pow(c,2));
  }
  vector<double> deltaF234_2term = {0, rho*(deltau - deltaV*n[0]), rho*(deltav - deltaV*n[1]), rho*(u*deltau + v*deltav - V*deltaV)};
  for (size_t i = 0; i < 4; i++) {
    deltaF234[i] = deltaF234[i]+deltaF234_2term[i];
  }

  vector<double> deltaF5 = {1, u+c*n[0], v+c*n[1], H+c*V};
  for (size_t i = 0; i < 4; i++) {
    deltaF5[i] = deltaF5[i]*abs(V+c)*(deltap + rho*c*deltaV)/(2*pow(c,2));
  }

  vector<double> flux_left = {left.rho*V_left,
                              left.rho*V_left*left.u + n[0]*left.p,
                              left.rho*V_left*left.v + n[1]*left.p,
                              left.rho*V_left*left.H};

  vector<double> flux_right = {right.rho*V_right,
                              right.rho*V_right*right.u - n[0]*right.p,
                              right.rho*V_right*right.v - n[1]*right.p,
                              right.rho*V_right*right.H};



  vector<double> Flux = {0,0,0,0};
  for (size_t i = 0; i < 4; i++) {
    Flux[i] = 0.5*(flux_left[i] + flux_right[i] - deltaF1[i] - deltaF234[i] - deltaF5[i]);
  }


  return Flux; //changer 0 pour residu
}

vector<double> CalculateW(){
  // Calcul de delta W
  return {0};//changer 0 pour W
}

void UpdateW(){

}

void UpdateGhostsCells(){

}
