//
//  connectivity.cpp
//  TestMesh
//
//  Created by Charles Dansereau on 2020-10-25.
//
#include "Solver.hpp"
using namespace std;


void Solve(FluxConvectifs *valeurs, variables_conservatrices* W){
  // appel des fonctions definies plus bas pour resoudre
  double deltat[NELEM];
  vector<double> v(4);
  vector<vector<double> > residu(NELEM,v);
  vector<double> deltaW[NELEM];
  vector<double> flux = {0,0,0,0};
  FluxConvectifs left;
  FluxConvectifs right;
  cout << NFACE << '\n';


  //iteration sur toutes les Faces
  for (size_t iface = 0; iface < NFACE; iface++) {
    cout << "face: " << iface << '\n';
    int iElem_left = face2el[2*iface];

    int iElem_right = face2el[2*iface+1];
    cout << "iElem_right: " << iElem_right << " u: " << valeurs[iElem_right].u <<'\n';

    //calcul des flux convectifs avec roe pour la face iface

    left = valeurs[iElem_left];
    cout << "test a "<< '\n';
    right = valeurs[iElem_right]; //cette ligne ne s'excute pas pour la face 3, element 3
    cout <<"  u left cell: "<< right.u << "  normalVecx: "<< normalVec[iElem_left][iface][0] <<'\n';
    flux = CalculateFlux(left, right, normalVec[iElem_left][iface]);
    //iteration sur les composantes de Fc pour les sommer au residu
    for (size_t i = 0; i < 4; i++) {
      residu[iElem_left][i] += flux[i]*deltaS[iElem_left][iface]; // le schema de roe multiplie-t-il deja par la normale?
      residu[iElem_right][i] -= flux[i]*deltaS[iElem_left][iface];
      cout << residu[iElem_left][i] << " (residu)" << '\n';
    }

  }
  //iteration sur tous les cellules
  for (size_t iElem = 0; iElem < NELEM; iElem++) {

    //preparation des valeurs
    W[iElem].Energy = ComputeEnergy(valeurs[iElem].rho, valeurs[iElem].u, valeurs[iElem].v, valeurs[iElem].p,gammaFluid);
    W[iElem].rho = valeurs[iElem].rho;
    W[iElem].rho_u = valeurs[iElem].rho * valeurs[iElem].u;
    W[iElem].rho_v = valeurs[iElem].rho * valeurs[iElem].v;
    W[iElem].rho_E = valeurs[iElem].rho * W[iElem].Energy;
    //calcul du deltat pour chaque element
    deltat[iElem] = CalculateDeltat(iElem, valeurs[iElem], area[iElem], normalVec[iElem]);
    deltaW[iElem] = CalculateW(iElem, deltat[iElem], area[iElem], residu[iElem]);
    UpdateW(iElem, W, deltaW[iElem]);
  }
}

double CalculateDeltat(int iElem, FluxConvectifs valeurs, double volume, vector<vector<double> > normal){
  double dt;
  double c;
  double SSx = 0;
  double SSy = 0;
  double RayonSpecX;
  double RayonSpecY;
  double RayonSpec;

  for (int iFace = 0; iFace < cell2nodeStart[iElem]; iFace++){
    SSx += normal[iFace][0];
    SSy += normal[iFace][1];
    c = pow(gammaFluid*valeurs.p/valeurs.rho,0.5);
    }
  RayonSpecX = 0.5*(abs(valeurs.u)+c)*SSx;
  RayonSpecY = 0.5*(abs(valeurs.v)+c)*SSy;
  RayonSpec = RayonSpecX+RayonSpecY;
  dt = CFL*volume/RayonSpec;
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
  double V_right = right.u*n[0] + right.v*n[1];
  double deltaV = V_left - V_right;
  double deltap = left.p - right.p;
  double deltarho = left.rho - right.rho;
  double deltau = left.u - right.u;
  double deltav = left.v - right.v;

  vector<double> deltaF1 = {1, u-c*n[0], v-c*n[0], H - c*V};
  for (size_t i = 0; i < 4; i++) {
    deltaF1[i] = deltaF1[i]*abs(V-c)*(deltap - rho*c*deltaV)/(2.0*pow(c,2));
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
    deltaF5[i] = deltaF5[i]*abs(V+c)*(deltap + rho*c*deltaV)/(2.0*pow(c,2));
  }

  // vector<double> flux_left = {left.rho*V_left,
  //                             left.rho*V_left*left.u + n[0]*left.p,
  //                             left.rho*V_left*left.v + n[1]*left.p,
  //                             left.rho*V_left*left.H};
  //
  // vector<double> flux_right = {right.rho*V_right,
  //                             right.rho*V_right*right.u - n[0]*right.p,
  //                             right.rho*V_right*right.v - n[1]*right.p,
  //                             right.rho*V_right*right.H};

  double rho_avg = 0.5*(right.rho + left.rho);
  double u_avg = 0.5*(right.u + left.u);
  double v_avg = 0.5*(right.v + left.v);
  double p_avg = 0.5*(right.p + left.p);
  double H_avg = 0.5*(right.H + left.H);
  double V_center = u_avg*n[0] + v_avg*n[1];


  vector<double> flux_center = {rho_avg*V_center,
                              rho_avg*V_center*u_avg - n[0]*p_avg,
                              rho_avg*V_center*v_avg - n[1]*p_avg,
                              rho_avg*V_center*H_avg};

  vector<double> Flux = {0,0,0,0};
  for (size_t i = 0; i < 4; i++) {
    Flux[i] = 0.5*(2.0*flux_center[i] - deltaF1[i] - deltaF234[i] - deltaF5[i]);
  }

  cout << "flux convectif[0]: " << Flux[0] << '\n';
  return Flux; //changer 0 pour residu
}

vector<double> CalculateW(int iElem, double dt, double volume, vector<double> Fc){
  // Calcul de delta W
  double deltaW[4];
  deltaW[iElem] = -dt * Fc[0] / volume;
  deltaW[iElem] = -dt * Fc[1] / volume;
  deltaW[iElem] = -dt * Fc[2] / volume;
  deltaW[iElem] = -dt * Fc[3] / volume;

  return {deltaW[0],deltaW[1],deltaW[2],deltaW[3]};
}

void UpdateW(int iElem, variables_conservatrices* produits, vector<double> deltaW){
  produits[iElem].rho   += deltaW[0];
  produits[iElem].rho_u += deltaW[1];
  produits[iElem].rho_v += deltaW[2];
  produits[iElem].rho_E += deltaW[3];
}

void UpdateGhostsCells(vector<double> FcBC, double volume){
  //boucler sur les faces externes copier les valeurs des elements internes
  // for (int iFace = 0; iFace < boundFace.size(); iFace++){
  //   int elem = face2el[2*iFace];
	//   double Area = face2Area[iFace];
  //
	// ... += FcBC[iFace].rho * volume;
	// ... += FcBC[iFace].u * volume;
	// ... += FcBC[iFace].v * volume;
	// ... += FcBC[iFace].H * volume;
	// }
}

double ComputeEnergy(double rho, double u, double v, double p, double gamma){
  return 0.5*rho*(pow(u,2)+pow(v,2)) + p * 1.0/(gamma-1.0);
}
double ComputePressure(double rho, double u, double v, double Energy, double gamma){
  return (gamma-1.0)*(rho*Energy-0.5*(pow(rho*u,2)+pow(rho*v,2))/rho);
}

double ComputeEnthalpy(double rho, double u, double v, double p, double gamma){
  return ComputeEnergy(rho,u,v,p,(1.0/(gamma-1.0))) + p/rho;
}
