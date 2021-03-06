//
//  connectivity.cpp
//  TestMesh
//
//  Created by Charles Dansereau on 2020-10-25.
//
#include "Solver.hpp"
using namespace std;


double Solve(FluxConvectifs *valeurs, variables_conservatrices* W, double erreur){
  // initialisation des valeurs
  erreur = 0;
  double deltat[nElemTot];
  vector<double> v(4);
  vector<vector<double> > residu(nElemTot,v);
  vector<double> deltaW[nElemTot];
  for (int i = 0; i < nElemTot;i++)
  {
          deltaW[i] = vector<double>(4);
  }
  vector<double> flux = {0,0,0,0};
  FluxConvectifs left;
  FluxConvectifs right;
  double nElem = NELEM;
  //cout << NFACE << '\n';


  //iteration sur toutes les Faces
  for (size_t iface = 0; iface < NFACE; iface++) {
    //cout << "face: " << iface << '\n';
    int iElem_left = face2el[2*iface];
    int facelocale;

    int iElem_right = face2el[2*iface+1];
    //cout << "iElem_right: " << iElem_right << " u: " << valeurs[iElem_right].u <<'\n';

    //calcul des flux convectifs avec roe pour la face iface

    left = valeurs[iElem_left];
    right = valeurs[iElem_right]; //cette ligne ne s'excute pas pour la face 3, element 3
    //cout <<"  u left cell: "<< right.u << "  normalVecx: "<< normalVec[iElem_left][iface][0] <<'\n';
    for (size_t i = 0; i < 4; i++) {
      if (fsuel[4*iElem_left+i] == iface) {
        facelocale = i;
      }
    }

    flux = CalculateFlux(left, right, normalVec[iElem_left][facelocale]);
    //iteration sur les composantes de Fc pour les sommer au residu
    for (size_t i = 0; i < 4; i++) {
      residu[iElem_left][i] -= flux[i]*deltaS[iElem_left][facelocale]; // le schema de roe multiplie-t-il deja par la normale?
      residu[iElem_right][i] += flux[i]*deltaS[iElem_left][facelocale]; //modifier ici pour la face locale
      //cout << residu[iElem_left][i] << " (residu)" << '\n';
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
    UpdateW(iElem, W, deltaW[iElem], valeurs);
    UpdateGhostsCells(valeurs, W);
    erreur += pow(deltaW[iElem][0],2);
  }
  erreur = sqrt(erreur/nElem);
  return erreur;
}

double CalculateDeltat(int iElem, FluxConvectifs valeurs, double volume, vector<vector<double> > normal){
  double dt;
  double c;
  double SSx = 0;
  double SSy = 0;
  double RayonSpecX;
  double RayonSpecY;
  double RayonSpec;


  c = pow(gammaFluid*valeurs.p/valeurs.rho,0.5);
  for (int iFace = 0; iFace < 4; iFace++){//etait cell2nodeStart[iElem]
    SSx += fabs(normal[iFace][0]*deltaS[iElem][iFace]);
    SSy += fabs(normal[iFace][1]*deltaS[iElem][iFace]);

    }
  RayonSpecX = 0.5*(fabs(valeurs.u)+c)*SSx;
  RayonSpecY = 0.5*(fabs(valeurs.v)+c)*SSy;
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
  double deltaV =  V_right - V_left;
  double deltap =  right.p - left.p;
  double deltarho =  right.rho - left.rho ;
  double deltau =   right.u - left.u;
  double deltav =  right.v - left.v;

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
  //cout<<flux_center[0]<<deltaF1[0]<<deltaF234[0]<<deltaF5[0]<<'\n';
  for (size_t i = 0; i < 4; i++) {
    Flux[i] = 0.5*(2.0*flux_center[i] - deltaF1[i] - deltaF234[i] - deltaF5[i]);
  }

  //cout << "flux convectif[0]: " << Flux[0] << '\n';
  return Flux; //changer 0 pour residu
}

vector<double> CalculateW(int iElem, double dt, double volume, vector<double> Fc){
  // Calcul de delta W
  double deltaW[4] = {0,0,0,0};
  //cout << "dt: "<<dt<<"Fc[0]: "<<Fc[0]<< "volume: "<< volume<<'\n';
  deltaW[0] = dt * Fc[0] / volume;
  deltaW[1] = dt * Fc[1] / volume;
  deltaW[2] = dt * Fc[2] / volume;
  deltaW[3] = dt * Fc[3] / volume;

  return {deltaW[0],deltaW[1],deltaW[2],deltaW[3]};
  //si probleme de stack smashing:
  //1. changer pour void et enlever le return
  //2. passer deltW de solve par reference
  //3. modifier directement deltaw par l'adresse
}

void UpdateW(int iElem, variables_conservatrices* produits, vector<double> deltaW, FluxConvectifs* valeurs){
  produits[iElem].rho   += deltaW[0];
  produits[iElem].rho_u += deltaW[1];
  produits[iElem].rho_v += deltaW[2];
  produits[iElem].rho_E += deltaW[3];

  valeurs[iElem].rho = produits[iElem].rho;
  valeurs[iElem].u = produits[iElem].rho_u/produits[iElem].rho;
  valeurs[iElem].v = produits[iElem].rho_v/produits[iElem].rho;
  produits[iElem].Energy = produits[iElem].rho_E/produits[iElem].rho;
  double gamma = 1.4;
  valeurs[iElem].p = ComputePressure(valeurs[iElem].rho, valeurs[iElem].u, valeurs[iElem].v, produits[iElem].Energy, gamma);
  valeurs[iElem].H = ComputeEnthalpy(valeurs[iElem].rho, valeurs[iElem].u, valeurs[iElem].v, valeurs[iElem].p, gamma );
  //mettre a jour la H et p en passant fluxconvectifs tous les champs dans flux convectifs
}



double ComputeEnergy(double rho, double u, double v, double p, double gamma){
  return (0.5*rho*(pow(u,2)+pow(v,2)) + p * 1.0/(gamma-1.0));
}
double ComputePressure(double rho, double u, double v, double Energy, double gamma){
  return ((gamma-1.0)*(rho*Energy-0.5*(pow(rho*u,2)+pow(rho*v,2))/rho));
}

double ComputeEnthalpy(double rho, double u, double v, double p, double gamma){
  return (ComputeEnergy(rho,u,v,p,(1.0/(gamma-1.0))) + p/rho);
}
