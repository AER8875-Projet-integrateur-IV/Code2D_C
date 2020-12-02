#include "connectivity.hpp"

//global variables
vector<int> face2el;

void Connectivity(){
  cout << "----- Starting Connectivity Calculations -----" << endl;

  int startI;
  int endI;
  int nodeI;

  MeshReader();
  cout << "reading done!" << '\n';
  cout << "NDIME  = " << NDIME << '\n';
  cout << "NELEM  = " << NELEM << '\n';
  cout << "NPOIN  = " << NPOIN << '\n';
  cout << "NFACE  = " << NFACE << '\n';

  cout << "cell2nodeStart: " << '\n';
  for (int i = 0; i < NELEM+1; i++){
    cout << cell2nodeStart[i] << " ";
  }
  cout << '\n';
  cout << "cell2node: " << '\n';
  for (int i = 0; i < NELEM+1; i++){
    for (int j = 0; j < cell2nodeStart[i+1]-cell2nodeStart[i]; j++){
      cout << cell2node[j+cell2nodeStart[i]] << "  ";
    }
    cout << '\n';
  }




  vector<int> esupStart;
  for (int i = 0; i < NPOIN+1; i++){
      esupStart.push_back(0);
  }

  cout << "Computing node to element connectivity...";

  for (int elementI = 0; elementI < NELEM; elementI++){
      startI = cell2nodeStart[elementI];
      endI = cell2nodeStart[elementI+1];

      for (int i=startI;i <endI;i++){
          nodeI = cell2node[i];
          esupStart.at(nodeI+1) += 1;
      }
  }

  for (int i=1; i<NPOIN+1;i++){
      esupStart.at(i) += esupStart[i-1];
  }



  vector<int> esup;
  for (int i = 0; i < esupStart.back(); i++){
    esup.push_back(0);
  }

  vector<int> store;
  for (int i=0;i<NPOIN;i++){
    store.push_back(0);
  }


  for (int elementI = 0; elementI < NELEM; elementI++){
      startI = cell2nodeStart[elementI];
      endI = cell2nodeStart[elementI+1];

       for (int i = startI; i < endI;i++) {
          nodeI = cell2node[i];
          int j = esupStart.at(nodeI) + store.at(nodeI);
          esup.at(j) = elementI;
          store.at(nodeI) += 1;
      }
  }

  cout<<"Computing node to element connectivity...Done!\n";
  cout<<"Node to element = " ;
  for (int i=0;i<esup.size();i++) cout<< esup.at(i)<< " ";
  cout << "size = " << esup.size();
  cout<<"\n==============================================\n";

  cout<<"Computing element 2 element connectivity...";

  vector<int> esuelStart;
  for(int i=0;i<NELEM+1;i++){
      esuelStart.push_back(0);
  }

  for (int elementI=0; elementI<NELEM; elementI++) {
      startI = cell2nodeStart[elementI];
      endI = cell2nodeStart[elementI+1];
      int nLocalFaces = endI - startI;
      esuelStart.at(elementI+1) += nLocalFaces;
  }

  for (int i=1; i<NELEM+1;i++) {
      esuelStart.at(i) += esuelStart[i-1];
  }

  vector<int> esuel;
  for(int i=0;i<esuelStart.back();i++) {
      esuel.push_back(-1);
  }
  vector<int> fsuel;
  for(int i=0;i<esuelStart.back();i++) {
      fsuel.push_back(-1);
  }

  for(int i=0;i<(NFACE * 2);i++) {
      face2el.push_back(-1);
  }
  int faceCount = 0;

  int lhelp [2];
  int lpoint [NPOIN];

  for (int elementI=0; elementI<NELEM; elementI++) {
      startI = cell2nodeStart[elementI];
      endI = cell2nodeStart[elementI+1];
      int nLocalFaces = endI - startI;

      for (int faceI=0; faceI<nLocalFaces;faceI++) {
          int nNodesForFaceI = 2;

      // Saving the nodes of the face to find
          lhelp[0] = cell2node[startI+faceI];
          if (faceI == nLocalFaces-1)
              lhelp[1] = cell2node[startI];
          else
              lhelp[1] = cell2node[startI+faceI+1];

          // Storing the nodes of the face
          lpoint[lhelp[0]] = 1;
          lpoint[lhelp[1]] = 1;

          // Choosing the first node to loop over its elements
          int ipoint = lhelp[0];

          int elemStart = esupStart[ipoint];
          int elemEnd = esupStart[ipoint+1];

          for (int j=elemStart;j<elemEnd;j++) {
              int elementJ = esup[j];

              if(elementJ != elementI) {
                  int startJ = cell2nodeStart[elementJ];
                  int endJ = cell2nodeStart[elementJ+1];
                  int nLocaleFacesJ = endJ - startJ;

                  for (int faceJ=0;faceJ<nLocaleFacesJ;faceJ++) {
                      int nNodesForFaceJ =2;

                      if(nNodesForFaceI == nNodesForFaceJ) {
                          int count =0;
                          for (int localNodeJ=0;localNodeJ<nNodesForFaceJ;localNodeJ++) {
                              int pointIndex = startJ+faceJ+localNodeJ;
                              if (faceJ == nLocaleFacesJ-1 && localNodeJ == nNodesForFaceJ-1)
                                  pointIndex = startJ;
                              count += lpoint[cell2node[pointIndex]];
                          }
                          if (count == nNodesForFaceI) {
                              esuel.at(esuelStart.at(elementI)+faceI) = elementJ;
                              if (esuel.at(esuelStart.at(elementJ)+faceJ) == elementI)
                                  fsuel.at(esuelStart.at(elementI)+faceI) = fsuel.at(esuelStart.at(elementJ)+faceJ);
                              else {
                                  fsuel.at(esuelStart.at(elementI)+faceI) = faceCount;
                                  fsuel.at(esuelStart.at(elementJ)+faceJ) = faceCount;
                                  if (faceCount*2+0<face2el.size())
                                      face2el.at(faceCount*2+0) = elementI;
                                  else
                                      face2el.push_back(elementI);
                                   if (faceCount*2+1<face2el.size())
                                      face2el.at(faceCount*2+1) = elementJ;
                                  else
                                      face2el.push_back(elementJ) ;
                                  faceCount += 1;
                              }
                          }
                      }
                  }
              }
          }
          for (int i=0;i<nNodesForFaceI;i++) {
              lpoint[lhelp[i]] = 0;
          }
      }
  }
  cout<<"Computing element 2 element connectivity...Done!\n";
  cout<<"Element to element = ";
  for (int i=0;i<esuel.size();i++) cout<<esuel.at(i) << " ";
  cout <<"\n";
  cout<<"element to face = ";
  for (int i=0;i<fsuel.size();i++) cout<<fsuel.at(i) << " ";
  cout<< "\n";

  // Parcours de esuel pour calculer le nombre d'elements au total
  int elemCount = NELEM;
  //faceCount = NFACE;
  int nElemTot;
  int nGhostCells;

  for (int i = 0; i < esuelStart[NELEM]; ++i) {
      if (esuel[i] == -1) {
          esuel[i] = elemCount;
          elemCount += 1;
      }
      if (fsuel[i] == -1) {
          fsuel[i] = faceCount;
          faceCount += 1;
      }

  }

  for (size_t i = 0; i < NELEM; i++) {
    for (size_t j = 0; j < 4; j++) {//remplacer 4 par nbfaces ielem
      if (fsuel[4*i+j] > 39) {//remplacer 39 par nb faces internes
        cout << "placer element " << i << " dans la face " << fsuel[4*i+j] << '\n';
        face2el[2*fsuel[4*i+j]] = i;
        //face2el[2*fsuel[4*i+j]+1] = boundary element;
      }
  }

  }
  // Passage dans face2element pour mettre a jour les ghost cells
	// int countGhostcells = NELEM;
	// for (int i = 0; i < 2*NFACE; ++i) {
	// 	if (face2el[i] == -1) {
	// 		face2el[i] = countGhostcells;
	// 		countGhostcells += 1;
	// 	}
	// }


  nElemTot = elemCount;
  nGhostCells = nElemTot - NELEM;





  // Parcours de fsuel pour calculer le nombre de faces au total
  cout<<"Element to element = ";
  for (int i=0;i<esuel.size();i++) cout<<esuel.at(i) << " ";
  cout <<"\n";
  cout<<"Face to element = ";
  for (int i=0;i<face2el.size();i++) cout<<face2el.at(i) << " ";
  cout<< "\n";
  cout<<"element to face = ";
  for (int i=0;i<fsuel.size();i++) cout<<fsuel.at(i) << " ";
  cout<< "\n";

  for (int i = 0; i<NPOIN; i++)
  {
    for (int j = 0; j<NDIME; j++)
    {
      cout << coord[i][j] << "  ";
    }
    cout << '\n';
  }

  for (size_t i = 0; i < nMark; i++) {
    cout << "mark: " << i+1 << '\n';
    for (size_t j = 0; j < nElemb[i]; j++) {
      for (size_t k = 0; k < nNodeb[j]; k++) {
        cout << bCond[i][j][k] << "  ";
      }
      cout << '\n';
    }
  }

  cout << "----- Connectivity calculations DONE -----" << '\n';
}
