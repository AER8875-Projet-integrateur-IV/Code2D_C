//
//  testmesh.cpp
//  TestMesh
//
//  Created by Gaston Banna on 2020-10-15.
//

#include <iostream>
#include <string.h> //je devais rajouter ca pour que le memset fonctionne sur linux
#include <limits.h>
#include <cmath>
#include <vector>
using namespace std;

int main() {
    //3.-) DECLARATION DES VARIABLES
    //Variables obtenues par le MeshReader/MeshGenerator (dans l'ordre d'apparition selon format SU2)
    int NDIME;             //Nombre de dimensions
    int NPOIN=13;         //Nombre de points (noeuds) du maillage
    double COORD;         //Table des coordonnees de chaque point du maillage (x,y,z)
    int NELEM=13;         //Nombre d'elements dans le maillage
    int NNODE=3;        //Nombre de noeuds par element, ATTENTION!! peut varier pour un maillage non structure. il est preferable de produire une table avec NNODE pour chaque element!
    int NFACE=25;
    int CONNEC[NELEM][NNODE]; //Table des connectivites entre les elements et les noeuds. La premiere colonne de chaque ligne decrit le type d'element (ligne, triangle, carre,etc.)
    int NMARK;             //Nombre de TAGS au bord du domaine
    string MARKER_TAG;     //Chaine de caractere decrivant le TAG du bord (ex: upper,lower,rigt,left) Il nous faut une convention pour l'ordre d'apparition!!
    int MARKER_ELEMS;     //Table signifiant le nombre d'elements de bord pour chaque TAG (ex: le premier TAG est lower, et il y a 3 elements dans le bas du domaine --> le premier element de MARKER_ELEMS=3)
    int CONNECB;         //Table des connectivites entres les noeuds pour les elements du bord du domaine. La premiere colonne de chaque ligne decrit le type d'element (ligne, triangle, carre,etc.)
    //
    //
    // Variables a generer avec la fonction Mesh
    int nunke=5;                 //Nombre d'inconnues par element
    double unkne[NELEM][nunke];    //Matrice des inconnues en fonction de l'element
    //Ajouter les conditions frontieres!!
    int esup1[NELEM*NNODE];
    memset( esup1, 0, NELEM*NNODE*sizeof(int) ); //Vecteur des elements qui entourent un point "elements surrounding point"
    int esup2[NPOIN+1];
    memset( esup2, 0, (NPOIN+1)*sizeof(int) ); //Vecteur de dimension connue qui sert a obtenir les locations dans le vecteur esup1
    int nesup[NPOIN];
    memset( nesup, 0, NPOIN*sizeof(int) ); //Vecteur indiquant le nombre d'elements qui entourent un point(noeud)

    //Variables utiles pour la generation de esup1,esup2,etc.
    int ipoil;                    //Variable utilisee pour creer esup1,esup2
    int ipoin;
    int istor;


    //test, initiation de CONNEC
    CONNEC[0][0]=1;
    CONNEC[0][1]=2;
    CONNEC[0][2]=6;
    CONNEC[1][0]=2;
    CONNEC[1][1]=7;
    CONNEC[1][2]=6;
    CONNEC[2][0]=2;
    CONNEC[2][1]=3;
    CONNEC[2][2]=7;
    CONNEC[3][0]=3;
    CONNEC[3][1]=8;
    CONNEC[3][2]=7;
    CONNEC[4][0]=3;
    CONNEC[4][1]=4;
    CONNEC[4][2]=8;
    CONNEC[5][0]=4;
    CONNEC[5][1]=9;
    CONNEC[5][2]=8;
    CONNEC[6][0]=4;
    CONNEC[6][1]=5;
    CONNEC[6][2]=9;
    CONNEC[7][0]=5;
    CONNEC[7][1]=10;
    CONNEC[7][2]=9;
    CONNEC[8][0]=7;
    CONNEC[8][1]=8;
    CONNEC[8][2]=13;
    CONNEC[9][0]=8;
    CONNEC[9][1]=12;
    CONNEC[9][2]=13;
    CONNEC[10][0]=8;
    CONNEC[10][1]=9;
    CONNEC[10][2]=12;
    CONNEC[11][0]=9;
    CONNEC[11][1]=11;
    CONNEC[11][2]=12;
    CONNEC[12][0]=9;
    CONNEC[12][1]=10;
    CONNEC[12][2]=11;

    cout << "La table CONNEC de l'exemple est: " << endl;

    for (int i=0;i<NELEM;++i){
        for (int j=0;j<3;++j){
            printf("%2d ",CONNEC[i][j]);
        }
        printf("\n");
    }


    //4.-) Debut du code

    //________________________________________________________ Elements surrounding points__________________________________________
    printf("______________________Debut code ESUP________________________\n\n" );
    //Generer esup1 et esup2
    //Element Pass 1
    
    int esupStart[NPOIN+1];
    memset( esupStart, 0, (NPOIN+1)*sizeof(int) );
    int numEleSurPoi[NPOIN];
    memset( numEleSurPoi, 0, (NPOIN+1)*sizeof(int) );
    
    for (int i=0;i<NELEM;++i){
        for (int j=0;j<NNODE;++j){
            ipoil=CONNEC[i][j];
            esup2[ipoil]=esup2[ipoil]+1;
        }
    }

    for (int i=0;i<NPOIN;++i){
        nesup[i]=esup2[i+1];
    }
    //Storage/reshuffling Pass 1
    for (int i=1;i<NPOIN+2;++i){
        esup2[i]=esup2[i]+esup2[i-1];
    }

    //Element Pass 2: Store the elements in esup1
    for (int i=0; i<NELEM; ++i){
        for (int j=0; j<NNODE; ++j){
            ipoin=CONNEC[i][j]-1;
            istor=esup2[ipoin]+1;
            esup2[ipoin]=istor;
            esup1[istor-1]=i+1;
        }
    }
    //Storage/reshuffling Pass 1
    for (int i=NPOIN+1;i>0;--i){
        esup2[i]=esup2[i-1];
    }
    esup2[0]=0;


    //Affichage des vecteurs crees
    cout << "esup1=";
    for (int i=0;i<NELEM*NNODE;++i){
        printf("%2d ",esup1[i] );
    }
    printf("\n\n");
    cout << "esup2=";
    for (int i=0;i<NPOIN+1;++i){
        printf("%2d ",esup2[i] );
    }
    printf("\n\n");
    cout << "nesup=";
    for (int i=0;i<NPOIN;++i){
        printf("%2d ",nesup[i] );
    }
    printf("\n\n");

    //Test element 5
    int indice=5;
    cout << "Les elements qui entourent le point " << indice << " debutent par l'indice " << esup2[indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
    cout << "Ces elements sont les elements: " << esup1[esup2[indice-1]] << ", " << esup1[esup2[indice]-1] << endl << endl;

    //Test element 9
    indice=9;
    cout << "Les elements qui entourent le point " << indice << " debutent par l'indice " << esup2[indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
    cout << "Ces elements sont les elements: " ;
    for (int i=esup2[indice-1];i<esup2[indice];++i){
        printf("%2d ", esup1[i]);
    }
    printf("\n\n");

    //
    //__________________________________________Points surrounding points ________________________________________________________
    printf("______________________Debut code PSUP_______________________________________\n\n");
    std::vector<int> psup1;
    int psup2[NPOIN+1];
    memset( psup2, 0, (NPOIN+1)*sizeof(int) );
    int npsup[NPOIN];
    memset( npsup, 0, NPOIN*sizeof(int) );
    int lpoin[NPOIN];
    memset( lpoin, 0, NPOIN*sizeof(int) );
    istor=0;
    int ielem=0;
    int jpoin=0;


    //Creation de psup1 et psup2
    for(int ipoin=0;ipoin<NPOIN;++ipoin){
        for(int iesup=esup2[ipoin];iesup<esup2[ipoin+1];++iesup){
            ielem=esup1[iesup]-1;
            for (int inode=0; inode<NNODE;++inode){
                jpoin=CONNEC[ielem][inode];
                if (jpoin != (ipoin+1) && lpoin[jpoin-1]!=(ipoin+1)){
                    ++istor;
                    psup1.push_back (jpoin);
                    lpoin[jpoin-1]=ipoin+1;
                }
            }
        }
        psup2[ipoin+1]=istor;
    }

    //Creation de npsup
    for (int i=0;i<NPOIN;++i){
        npsup[i]=psup2[i+1]-psup2[i];
    }


    cout << "psup1=";
    for (int i=0;i<psup1.size();++i){
        printf("%2d ",psup1[i] );
    }
    printf("\n\n");
    cout << "psup2=";
    for (int i=0;i<NPOIN+1;++i){
        printf("%2d ",psup2[i] );
    }
    printf("\n\n");
    cout << "npsup=";
    for (int i=0;i<NPOIN;++i){
        printf("%2d ",npsup[i] );
    }
    printf("\n\n");
    cout << "lpoin=";
    for (int i=0;i<NPOIN;++i){
        printf("%2d ",lpoin[i] );
    }
    printf("\n\n");

    //Test point 5
    indice=5;
    cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << psup2[indice-1] << " et terminent par l'indice " << psup2[indice]-1 << " dans le vecteur psup1." << endl;
    cout << "Ces points sont les points: " ;
    for (int i=psup2[indice-1];i<psup2[indice];++i){
        printf("%2d ", psup1[i]);
    }
    printf("\n\n");


    //Test point 9
    indice=9;
    cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << psup2[indice-1] << " et terminent par l'indice " << psup2[indice]-1 << " dans le vecteur psup1." << endl;
    cout << "Ces points sont les points: " ;
    for (int i=psup2[indice-1];i<psup2[indice];++i){
        printf("%2d ", psup1[i]);
    }
    printf("\n\n");

    //___________________________________________ Elements surrounding Elements____________________________________________
    printf("_________________Debut code ESUEL________________________\n\n" );
    for (int i=0;i<NPOIN;++i){
        lpoin[i]=0;
    }
    int NFAEL=3;  //Cas particulier, dans notre code, il faudrait un vecteur avec le nombre de faces de chaque element
    int esuel[NELEM][NFAEL];
    memset(esuel, -1, sizeof esuel);
    int fsuel[NELEM][NFAEL];
    memset(fsuel, -1, sizeof fsuel);
    int face2el[NFACE][2];
    memset(face2el, -1, sizeof face2el);
    int faceCount=0;

    // esuel[0][2]=0 ;//weird cette valeur donnait -06785434??

    //Declaration des variables utiles pour la creation de esuel
    int nnofa=2;
    int lhelp[nnofa];

    int jelem;
    int nnofj;
    int icoun;

    //Creation de lpofa une matrice (ou liste) des points pour chaque face, specifique au type d'element, independant au maillage --> peut etre cree d'avance
    int lpofa[NFAEL][nnofa];
    memset(lpofa, 0, sizeof lpofa);

    lpofa[0][0]=1;
    lpofa[0][1]=2;
    lpofa[1][0]=2;
    lpofa[1][1]=3;
    lpofa[2][0]=3;
    lpofa[2][1]=1;

    //Creation de lnofa
    int lnofa[NFAEL];//={nnofa,nnofa,nnofa};
    //memset( lnofa, nnofa, NFAEL*sizeof(int) );

    for (int i=0;i<NFAEL;++i) {
        lnofa[i]=nnofa;
    }

    // Code ESUEL________________________


    for (int ielem=0;ielem<NELEM;++ielem){    // Looping over the elements
        for(int ifael=0;ifael<NFAEL;++ifael){ // Looping over the local face of element ielem
            nnofa=lnofa[ifael];
            for (int i=0;i<nnofa;++i){        // Looping over the nodes of face ifael
                lhelp[i]=CONNEC[ielem][lpofa[ifael][i]-1];
            }
            for (int i=0;i<nnofa;++i){        // Storing the nodes of the face
                lpoin[lhelp[i]-1]=1;
            }
            ipoin=lhelp[0];
            for (int istor=esup2[ipoin-1];istor<esup2[ipoin]-1;++istor){
                //Looping over the elements connected to ipoin
                jelem=esup1[istor];
                if (jelem-1 != ielem){
                    for (int jfael=0;jfael<NFAEL;++jfael){
                        nnofj=lnofa[jfael];
                        if (nnofj == nnofa){
                            icoun=0;
                            for (int jnofa=0;jnofa<nnofa;++jnofa){
                                jpoin=CONNEC[jelem-1][lpofa[jfael][jnofa]-1];
                                icoun+=lpoin[jpoin-1];
                            }
                            if (icoun == nnofa){
                                esuel[ielem][ifael]=jelem;
                            }
                        }
                    }
                }
            }
            for (int i=0;i<nnofa;++i){
                lpoin[lhelp[i]-1]=0;
            }
        }
    }

    cout << "La table esuel de l'exemple est: " << endl;

    for (int i=0;i<NELEM;++i){
        for (int j=0;j<NFAEL;++j){
            printf("%2d ",esuel[i][j]);
        }
        printf("\n");
    }
    int numNodeElem[NPOIN];
    memset( numNodeElem, 0, NPOIN*sizeof(int) );
    for (int i=0;i<NPOIN;++i){
        numNodeElem[i]=3;
    int sumTot = 0;
    for(int i=0;i<NPOIN;++i){
        sumTot+=numNodeElem[i];
    }
    int cell2nodeStart[NPOIN+1];
    memset( cell2nodeStart, 0, (NELEM+1)*sizeof(int) );
        int cell2node[sumTot];
        memset( cell2node, 0, sumTot*sizeof(int) );

            for (int i=0;i<NELEM;++i){
                cell2nodeStart[i+1]=cell2nodeStart[i]+numNodeElem[i];
                for (int j=0;j<numNodeElem[i];++j){
                    cell2node[cell2nodeStart[i]+j]=CONNEC[i][j];
                }
            }
            int numNodes=0;
            for (int i=0;i<cell2nodeStart[NPOIN];++i){
                if (cell2node[i]+1>numNodes){
                    numNodes=cell2node[i]+1;
                }
            }

    //Test point 5
    indice=5;
    //cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << esuel[][indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
    cout << "Les éléments entourant l'élément " << indice << " sont : " ;
    for (int i=0;i<NFAEL;++i){
        printf("%2d ", esuel[indice-1][i]);
    }
    printf("\n\n");


    //Test point 9
    indice=9;
    cout << "Les éléments entourant l'élément " << indice << " sont : " ;
    for (int i=0;i<NFAEL;++i){
        printf("%2d ", esuel[indice-1][i]);
    }
    printf("\n\n");

    // Initializing the node to element linked list connectivity
    int esup[sumTot];
    memset( esup, 0, sumTot*sizeof(int) );
        // Array to save the increment
        int store[NPOIN];
        memset( store, 0, NPOIN*sizeof(int) );

        for (int elemi=0;elemi<NELEM;++elemi){
            int startI=cell2nodeStart[elemi];
            int endI=cell2nodeStart[elemi+1];

            for (int i=startI;i<endI;++i){
                int nodeI=cell2node[i];
                // Fetching the index for the connectivity list
                int j=esupStart[nodeI]+store[nodeI];
                // Adding the element index for nodeI
                esup[j] = elemi;
                store[nodeI]+=1;
            }
        }


    //___________________________________________ Elements surrounding Elements____________________________________________
        printf("_________________Debut code FSUEL________________________\n\n");
        // Element 2 element start offset linked list
        int esuelStart[NELEM+1];
        memset( esuelStart, 0, (NELEM+1)*sizeof(int) );
        int fsuelStart[NELEM+1];
        memset( fsuelStart, 0, (NELEM+1)*sizeof(int) );

        // Counting the number of faces per elements
        int nLocalFaces;
        int startI;
        int endI;
        int nodeI;
        memset( cell2nodeStart, 0, (NELEM+1)*sizeof(int) );
        int numNodeElem[NPOIN];
        memset( numNodeElem, 0, NPOIN*sizeof(int) );
        for (int i=0;i<NPOIN;++i){
            numNodeElem[i]=3;
        }
        for(int i=0;i<NPOIN;++i){
            sumTot+=numNodeElem[i];
        }
    
        for (int elemi=0;elemi<NELEM;++elemi){
            startI = cell2nodeStart[elemi];
            endI = cell2nodeStart[elemi+1];
            nLocalFaces = endI-startI;
            esuelStart[elemi+1]+=nLocalFaces;
        }

        // Final setup of the element 2 element start offset linked list
        for (int i=1;i<NELEM+1;++i){
            esuelStart[i]+=esuelStart[i-1];
        }
        for (int i = 0; i < NELEM + 1; ++i) {
            fsuelStart[i] = esuelStart[i];
        }

        // Initializing the element 2 element linked list to -1 using fill_n from <algorithm>
        int esuel[esuelStart[NELEM]];
        std::fill_n(esuel,esuelStart[NELEM],-1);
        // Initializing the face surrounding element linked list to -1
        int fsuel[esuelStart[NELEM]];
        std::fill_n(fsuel, esuelStart[NELEM], -1);

        // Array to save information to speedup the process
        int nNodesForFace=2; // In 2D a face necesseraly has 2 nodes. this needs to be changed for a 3D mesh
        //int lhelp[nNodesForFace]={0};
        int lpoint[NPOIN];
        memset( lpoint, 0, NPOIN*sizeof(int) );
        int ipoint;
        int elemStart;
        int elemEnd;
        int elemj;
        int startJ;
        int endJ;
        int nNodesForFaceI;
        int nNodesForFaceJ;
        int count;
        int pointIndex;
        int nLocaleFacesJ;
        //int faceCount = 0;

        std::vector<int> face2node;
        std::vector<int> face2el;

        // Looping over all elements in the mesh
        for (int elemi=0;elemi<NELEM;++elemi)
        {
            startI = cell2nodeStart[elemi];
            endI = cell2nodeStart[elemi+1];
            nLocalFaces = endI - startI;


            // Looping over the local faces of elemi
            for (int faceI=0;faceI<nLocalFaces;++faceI){
                // In 2D, a face has 2 nodes
                nNodesForFaceI = nNodesForFace;// to be changed for 3D nNodesForFace to be an array

                //Saving the nodes of the face to find
                lhelp[0] = cell2node[startI+faceI];
                if (faceI == nLocalFaces-1){
                    lhelp[1] = cell2node[startI];
                }
                else {
                    lhelp[1] = cell2node[startI+faceI+1];
                }

                // Storing the nodes of the face
                lpoint[lhelp[0]] = 1;
                lpoint[lhelp[1]] = 1;

                // Choosing the first node to loop over its Elements
                ipoint = lhelp[0];

                elemStart = esupStart[ipoint];
                elemEnd = esupStart[ipoint+1];


                // Looping over the elements connected to point ipoint
                for (int j=elemStart;j<elemEnd;++j){
                    elemj = esup[j];

                    if (elemj != elemi){
                        startJ = cell2nodeStart[elemj];
                        endJ = cell2nodeStart[elemj+1];
                        nLocaleFacesJ = endJ - startJ;

                        // Looping over the faces of elementJ
                        for (int facej=0;facej<nLocaleFacesJ;++facej){
                            nNodesForFaceJ = 2; // Dont forget to change to an array in 3D! nNodesforFace[elemj]

                            if (nNodesForFaceI == nNodesForFaceJ){
                                count = 0;
                                for (int localNodeJ=0;localNodeJ<nNodesForFaceJ;++localNodeJ){
                                    pointIndex = startJ+facej+localNodeJ;

                                    if (facej == (nLocaleFacesJ-1) && localNodeJ == (nNodesForFaceJ-1)){
                                        pointIndex = startJ;

                                    }
                                    count += lpoint[cell2node[pointIndex]];
                                // If the number of matching nodes is equal to the number of nodes in faceI, faceJ is a match with faceI
                                if (count == nNodesForFaceI){
                                    //Adding elementJ to the connectivity of elementI
                                    esuel[esuelStart[elemi]+faceI]=elemj;

                                    // Checking if the face has already been added
                                    if (esuel[esuelStart[elemj] + facej] == elemi) {
                                        fsuel[esuelStart[elemi] + faceI] = fsuel[esuelStart[elemj] + facej];
                                    }
                                    // Adding a new face
                                    else {
                                        fsuel[esuelStart[elemi] + faceI] = faceCount;
                                        fsuel[esuelStart[elemj] + facej] = faceCount;
                                        face2el.push_back (elemi);
                                        face2el.push_back (elemj);
                                        for (int i=0;i<nNodesForFaceI;++i){
                                            face2node.push_back (lhelp[i]);
                                        }
                                        faceCount += 1;
                                    }
                                }

                                }


                            }
                        }
                    }
                }

                for (int i=0;i<nNodesForFaceI;++i){
                    lpoint[lhelp[i]]=0;
                }
            }
        }


        printf("face2node = \n");
        for (int i = 0; i < face2node.size(); ++i) {
            printf("%2d ", face2node[i]);
        }
        printf("\n\n");

        // Parcours de esuel pour calculer le nombre d'elements au total
        int elemCount = NELEM;
        int nElemTot;
        int nGhostCells;

        for (int i = 0; i < esuelStart[NELEM]; ++i) {
            if (esuel[i] == -1) {
                esuel[i] = elemCount;
                elemCount += 1;
            }
        }
        nElemTot = elemCount;
        nGhostCells = nElemTot - NELEM;


        int nIntFaces = faceCount;// Nombre de faces interne
        int nFaces;               // Nombre total de nFaces
        int nBondFaces;           // Nombre de faces frontieres


        // Parcours de fsuel pour calculer le nombre de faces au total

        for (int i = 0; i < esuelStart[NELEM]; ++i) {
            if (fsuel[i] == -1) {
                fsuel[i] = faceCount;
                faceCount += 1;
            }
        }

        nFaces = faceCount;
        nBondFaces = nFaces - nIntFaces;

        // Initializing the face2element connectivity
        int condition;
        int condition2;
        int condition3;
        int compteur;
        int k;
        int indiceFace[2] = {0};
        int elemi;

        for (int faceI = nIntFaces; faceI < nFaces; ++faceI) {//Looping over the faces
            compteur = 0;
            condition = 1;
            i = 0;
            while (condition) {
                if (fsuel[i] == faceI) {
                    indiceFace[compteur] = i;
                    compteur += 1;
                }
                if (compteur == 2) {
                    condition = 0;
                } else if (compteur == 1 && i > fsuelStart[NELEM]) {
                    indiceFace[compteur] = fsuelStart[NELEM];
                    condition = 0;
                } else {
                    i += 1;
                }
            }
            condition2 = 1;
            int j = 0;
            while (condition2) {
                if (fsuelStart[j] <= indiceFace[0] && fsuelStart[j + 1] > indiceFace[0]) {
                    elemi = j;
                    condition2 = 0;
                } else {
                    j += 1;
                }
            }
            condition3 = 1;
            k = 0;
            while (condition3) {
                if (fsuelStart[k] <= indiceFace[1] && fsuelStart[k + 1] > indiceFace[1]) {
                    elemj = k;
                    condition3 = 0;
                } else {
                    k += 1;
                }

                if (k == NELEM) {
                    elemj = -1;
                    condition3 = 0;
                }
            }
            face2el.push_back (elemi);
            face2el.push_back (elemj);
        }


        // Passage dans face2el pour mettre a jour les ghost cells
        int countGhostcells = NELEM;
        for (int i = 0; i < 2 * nFaces; ++i) {
            if (face2el[i] == -1) {
                face2el[i] = countGhostcells;
                countGhostcells += 1;
            }
        }

        // Passage dans face2node pour mettre a jour les ghost cells
        countGhostcells = NELEM;
        int trouve;
        //int depasse=0;
        for (int elemi=0;elemi<NELEM;++elemi){
            startI=cell2nodeStart[elemi];
            endI=cell2nodeStart[elemi+1];
            nLocalFaces = endI - startI;

            for (int faceI=0;faceI<nLocalFaces;++faceI){// En 2D, cette ligne est vraie,
            // en 3D, ce n'est pas vrai que le nombre de noeuds dun element correspond
            //au nombre de faces, il faut donc penser a creer, en plus de numNodeElem,
            // un vecteur qui va stocker le nombre de faces par element!!
            // for (int faceI=0;faceI<numFaceElem[elemi])...
                lhelp[0]=cell2node[startI+faceI];
                if (faceI == nLocalFaces-1){
                    lhelp[1] = cell2node[startI];
                }
                else {
                    lhelp[1] = cell2node[startI+faceI+1];
                }
                printf(" lhelp[0]= %2d\n", lhelp[0]);
                printf(" lhelp[1]= %2d\n", lhelp[1]);

                trouve=0;
                for (int i=0;i<face2node.size()/2;++i){
                    if ((face2node[i*nNodesForFace] == lhelp[0] && face2node[i*nNodesForFace+1] == lhelp[1]) || (face2node[i*nNodesForFace] == lhelp[1] && face2node[i*nNodesForFace+1] == lhelp[0])){
                        trouve=1;
                    }
                }

                if (!trouve){
                    face2node.push_back (lhelp[0]);
                    face2node.push_back (lhelp[1]);
                }
            }
        }


        //___________________________________________________________________________
        // Affichage des linked lists

        printf("esuelStart = \n");
        for (int i=0;i<NELEM+1;++i){
            printf("%2d ",esuelStart[i] );
        }
        printf("\n\n");

        printf("esuel = \n");
        for (int i=0;i<esuelStart[NELEM];++i){
            printf("%2d ",esuel[i] );
        }
        printf("\n\n");

        printf("Nombre d'elements internes = %2d\n", NELEM);
        printf("Nombre d'elements total = %2d\n", nElemTot);
        printf("Nombre de cellules fantomes = %2d\n", nGhostCells);
        printf("\n\n");

        printf("fsuelStart = \n");
        for (int i = 0; i < NELEM + 1; ++i) {
            printf("%2d ", fsuelStart[i]);
        }
        printf("\n\n");

        printf("fsuel = \n");
        for (int i = 0; i < esuelStart[NELEM]; ++i) {
            printf("%2d ", fsuel[i]);
        }
        printf("\n\n");

        printf("Nombre de faces internes = %2d\n", nIntFaces);
        printf("Nombre de faces  = %2d\n", nFaces);
        printf("Nombre de faces frontieres = %2d\n", nBondFaces);
        printf("\n\n");

        printf("face2el = \n");
        for (int i = 0; i < 2 * nFaces; ++i) {
            printf("%2d ", face2el[i]);
        }
        printf("\n\n");

        printf("face2node = \n");
        for (int i = 0; i < 2 * nFaces; ++i) {
            printf("%2d ", face2node[i]);
        }
        printf("\n\n");
    }

    //Test point 5
    indice=5;
    //cout << "Les points qui entourent le point " << indice << " debutent par l'indice " << esuel[][indice-1] << " et terminent par l'indice " << esup2[indice]-1 << " dans le vecteur esup1." << endl;
    cout << "Les éléments entourant l'élément " << indice << " sont : " ;
    for (int i=0;i<NFAEL;++i){
        printf("%2d ", fsuel[indice-1][i]);
    }
    printf("\n\n");


    //Test point 9
    indice=9;
    cout << "Les éléments entourant l'élément " << indice << " sont : " ;
    for (int i=0;i<NFAEL;++i){
        printf("%2d ", fsuel[indice-1][i]);
    }
    printf("\n\n");

}
