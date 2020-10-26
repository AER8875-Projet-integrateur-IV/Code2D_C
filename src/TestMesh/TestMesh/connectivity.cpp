//
//  connectivity.cpp
//  TestMesh
//
//  Created by Gaston Banna on 2020-10-25.
//

#include "connectivity.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int nPoints = 15;
    int nElements = 13;
    int nFaces = 27;
    
    int cell2nodeStart [] = {0,4,8,12,16,19,22,25,28,31,34,37,40,43};
    int cell2node [] = {0,1,6,5,1,2,7,6,2,3,8,7,3,4,9,8,8,9,10,13,11,12,8,10,11,7,11,13,6,7,13,14,6,13,14,13,12,5,6,14,7,8,11};
    
        0,1,6,5,1,2,7,6,2,3,8,7,3,4,9,8,8,9,10,13,11,12,8,10,11,7,11,13,6,7,13,14,6,13,14,13,12,5,6,14,7,8,11
    
    int startI;
    int endI;
    int nodeI;
    
    vector<int> esupStart;
    for (int i=0;i<nPoints+1;i++) {
        esupStart.push_back(0);
    }
    
    cout << "Computing node to element connectivity...";
    
    for (int elementI=0; elementI<nElements;elementI++) {
        startI = cell2nodeStart[elementI];
        endI = cell2nodeStart[elementI+1];
        
        for (int i=startI;i <endI;i++) {
            nodeI = cell2node[i];
            esupStart.at(nodeI+1) += 1;
        }
    }
    
    for (int i=1; i<nPoints+1;i++) {
        esupStart.at(i) += esupStart[i-1];
    }
    
    vector<int> esup;
    for (int i=0;i<esupStart.back();i++)
        esup.push_back(0);
    vector<int> store;
    for (int i=0;i<nPoints;i++)
        store.push_back(0);
    
    for (int elementI=0; elementI<nElements;elementI++) {
        startI = cell2nodeStart[elementI];
        endI = cell2nodeStart[elementI+1];
        
         for (int i=startI;i <endI;i++) {
            nodeI = cell2node[i];
            int j = esupStart.at(nodeI) + store.at(nodeI);
            cout <<j<<" ";
            esup.at(j) = elementI;
            store.at(nodeI) += 1;
        }
    }
    cout<<"Computing node to element connectivity...Done!\n";
    cout<<"Node to element = " ;
    for (int i=0;i<sizeof(esup);i++) cout<< esup.at(i)<< " ";
    cout<<"\n==============================================\n";
    
    cout<<"Computing element 2 element connectivity...";
    
    vector<int> esuelStart;
    for(int i=0;i<nElements+1;i++){
        esuelStart.push_back(0);
    }
    
    for (int elementI=0; elementI<nElements; elementI++) {
        startI = cell2nodeStart[elementI];
        endI = cell2nodeStart[elementI+1];
        int nLocalFaces = endI - startI;
        esuelStart.at(elementI+1) += nLocalFaces;
    }
    
    for (int i=1; i<nElements+1;i++) {
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
    vector<int> face2el;
    for(int i=0;i<(nFaces * 2);i++) {
        face2el.push_back(-1);
    }
    int faceCount = 0;
    
    int lhelp [2];
    int lpoint [nPoints];
    
    for (int elementI=0; elementI<nElements; elementI++) {
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
                                        face2el.at(faceCount*2+0) = elementJ;
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
    for (int i=0;i<sizeof(esuel);i++) cout<<esuel.at(i) << " ";
    cout <<"\n";
    cout<<"Face to element = ";
    for (int i=0;i<sizeof(fsuel);i++) cout<<fsuel.at(i) << " ";
    cout<< "\n";
    
    // Parcours de esuel pour calculer le nombre d'elements au total
    int elemCount = nElements;
        faceCount = nFaces;
    int nElemTot;
    int nGhostCells;

    for (int i = 0; i < esuelStart[nElements]; ++i) {
        if (esuel[i] == -1) {
            esuel[i] = elemCount;
            elemCount += 1;
        }
        if (fsuel[i] == -1) {
            fsuel[i] = faceCount;
            faceCount += 1;
        }
    }
    nElemTot = elemCount;
    nGhostCells = nElemTot - nElements;


    //int nFaceInt = faceCount;// Nombre de faces interne
    //int nBondFaces;           // Nombre de faces frontieres


    // Parcours de fsuel pour calculer le nombre de faces au total
    cout<<"Element to element = ";
    for (int i=0;i<sizeof(esuel);i++) cout<<esuel.at(i) << " ";
    cout <<"\n";
    cout<<"Face to element = ";
    for (int i=0;i<sizeof(fsuel);i++) cout<<fsuel.at(i) << " ";
    cout<< "\n";

    int cell2nodeEnd
    int npoints
    for (int i = 0; i < cell2nodeStart[nElements]; ++i) {
        cell2nodeEnd = cell2nodeStart[i+1]-1;
        cell2node[i] = elemCount;
        elemCount += 1;
        npoints = cell2nodeEnd - cell2nodeStart[i] +1;
        if npoints == 3 {             //formules triangke
            //omega = 1/2*(x*(cell2node[cell2nodeStart[i]]) - x(cell2node[cell2nodeStart[i]+1]))(y1-y2)
        }         else if npoints == 4 {             //formule carr/
        }
        else {
            //pas un triangle ou carr/
        }
             
    }
    
    
    return 0;
}
