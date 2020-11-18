//
//  connectivity.cpp
//  TestMesh
//
//  Created by Charles Dansereau on 2020-10-25.
//

#include "Solver.hpp"
#include "MeshReader.cpp"
#include "Metriques.cpp"

using namespace std;

double CalculateDeltat(double volume){
  //calcul de delta t ici
  return 0; //changer 0 pour delta t
}

double CalculateResidual(){
  //calcul du r/sidu ici c'est ici que va le roe scheme
  return 0; //changer 0 pour residu
}

vector<double> CalculateW(){
  // Calcul de delta W
  return {0};//changer 0 pour W
}

void UpdateW(){

}

void UpdateGhostsCells(){

}
