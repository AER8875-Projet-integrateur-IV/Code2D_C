#include "metrics.hpp"
#include <limits>

vector<double> area;
vector<vector<double> > centroidVec;
vector<vector<vector<double> > > normalVec;
vector<vector<double> > deltaS;

void Metrics(){
  cout << "----- Starting Metriques Calculations -----" << endl;

  centroidVec.resize(NELEM);

  int num_rows = NELEM;
  int num_cols = NDIME;

  double b = 1;

  // Loop over the elements of the Mesh object
  for (int iElem = 0; iElem<NELEM; iElem++){

    // Verify if the element is a triangle

    int taille = cell2nodeStart[iElem+1]-cell2nodeStart[iElem];
    if (taille == 3){

      // Find what nodes compose this element
      int node1 = cell2node[cell2nodeStart[iElem]];
      int node2 = cell2node[cell2nodeStart[iElem]+1];
      int node3 = cell2node[cell2nodeStart[iElem]+2];

      // Find each node's coordinates
      vector<double> coord1 = {coord[node1][0],coord[node1][1]};
      vector<double> coord2 = {coord[node2][0],coord[node2][1]};
      vector<double> coord3 = {coord[node3][0],coord[node3][1]};

      area.push_back(abs(b/2*((coord1[0]-coord2[0])*(coord1[1]+coord2[1])
                    +(coord2[0]-coord3[0])*(coord2[1]+coord3[1])
                    +(coord3[0]-coord1[0])*(coord3[1]+coord1[1]))));

      centroidVec[iElem].push_back(1/3*(coord1[0]+coord2[0]+coord3[0]));
      centroidVec[iElem].push_back(1/3*(coord1[1]+coord2[1]+coord3[1]));
      cout << centroidVec[iElem][0] << '\n';
      cout << centroidVec[iElem][1] << '\n';

      // Calculate the normal vector to each face

    }
    // Verify if the element is a quadrelateral
    else if (taille == 4) {
      // Find what nodes compose this element
      int node1 = cell2node[cell2nodeStart[iElem]];
      int node2 = cell2node[cell2nodeStart[iElem]+1];
      int node3 = cell2node[cell2nodeStart[iElem]+2];
      int node4 = cell2node[cell2nodeStart[iElem]+3];
      // Find each node's coordinates
      vector<double> coord1 = {coord[node1][0],coord[node1][1]};
      vector<double> coord2 = {coord[node2][0],coord[node2][1]};
      vector<double> coord3 = {coord[node3][0],coord[node3][1]};
      vector<double> coord4 = {coord[node4][0],coord[node4][1]};
      // Store each calculated area in array
      area.push_back(abs(b/2*((coord1[0]-coord3[0])*(coord2[1]-coord4[1])
                    +(coord4[0]-coord2[0])*(coord1[1]-coord3[1]))));
      // Define the two triangles from the decomposition of the quadrilateral element
      double area123 = abs(b/2*((coord1[0]-coord2[0])*(coord1[1]+coord2[1])
                    +(coord2[0]-coord3[0])*(coord2[1]+coord3[1])
                    +(coord3[0]-coord1[0])*(coord3[1]+coord1[1])));
      double area134 = abs(b/2*((coord1[0]-coord3[0])*(coord1[1]+coord3[1])
                +(coord3[0]-coord4[0])*(coord3[1]+coord4[1])
                +(coord4[0]-coord1[0])*(coord4[1]+coord1[1])));

      // Define the centroid of each sub-triangle
      vector<double> centroid123 =
      {
        1.0/3.0*(coord1[0]+coord2[0]+coord3[0]),
        1.0/3.0*(coord1[1]+coord2[1]+coord3[1])
      };
      vector<double> centroid134 =
      {
        1.0/3.0*(coord1[0]+coord3[0]+coord4[0]),
        1.0/3.0*(coord1[1]+coord3[1]+coord4[1])
      };

      centroidVec[iElem].push_back((area123*centroid123[0]+area134*centroid134[0])/(area123+area134));
      centroidVec[iElem].push_back((area123*centroid123[1]+area134*centroid134[1])/(area123+area134));
      //cout << centroidVec[iElem][0] << ' ' << centroidVec[iElem][1] << '\n';

    }
    // Print error message if the element type is wrong
    else {
      cout << "[ERROR] : Element is not a triangle or a quadrilateral. Cannot treat element." << endl;
    }



  }


  // Resize the normal vector to match the number of elements
  normalVec.resize(NELEM);
  deltaS.resize(NELEM);

  for (int iElem = 0; iElem<NELEM; iElem++){
    // Find the number of points inside the element
    int nbPoint = cell2nodeStart[iElem+1]-cell2nodeStart[iElem];

    // Resize the next vector in the normal array
    normalVec[iElem].resize(nbPoint); // True because in 2D number of faces = number of points
    deltaS[iElem].resize(nbPoint);
    // Store in a vector the nodes associated with an element
    // Store for a second time the first point at the end of the vector to
    // calculate the last normal vector


    vector<int> numPt;
    numPt.resize(nbPoint);
    for (size_t i = 0; i < nbPoint; i++) {
      numPt[i] = cell2node[cell2nodeStart[iElem]+i];
    }
    numPt.push_back(cell2node[cell2nodeStart[iElem]]);

    // Loop over the number of points to calculate the normals
    for (int i = 0; i<nbPoint; i++){
      vector<double> coord1 = coord[numPt[i]];
      vector<double> coord2 = coord[numPt[i+1]];
      deltaS[iElem][i] = pow(coord2[1]-coord1[1],2)+pow(coord1[0]-coord2[0],2);
      normalVec[iElem][i].push_back((coord2[1]-coord1[1])/sqrt(pow(coord2[1]-coord1[1],2)+pow(coord1[0]-coord2[0],2)));
      normalVec[iElem][i].push_back((coord1[0]-coord2[0])/sqrt(pow(coord2[1]-coord1[1],2)+pow(coord1[0]-coord2[0],2)));

      // Verify normal coordinates
    //   for (int j = 0; j<2; j++){
    //     cout << normalVec[iElem][i][j] << ' ';
    //   }
    //   cout << "\n" << '\n';
    //   cout << "===========" << '\n';
    //   //cout << "----- Test1 -----" << endl;
    }

    // Test Integral sur les surfacesS
    double test_x = 0;
    double test_y = 0;
    for (size_t i = 0; i < nbPoint; i++) {
      test_x += normalVec[iElem][i][0];
      test_y += normalVec[iElem][i][1];
    }
    //cout << test_x << '\n';
    //cout << test_y << '\n';
  }

  cout << "----- Metriques calculations DONE -----" << '\n';
}
