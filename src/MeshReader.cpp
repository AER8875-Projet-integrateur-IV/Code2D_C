// reading a text file
#include "MeshReader.hpp"

//global VARIABLES
vector<vector<double> > coord;
int NDIME;
int NPOIN;
int NELEM;
int NFACE;
vector<int> cell2node;
vector<int> cell2nodeStart = {0};
vector<int> nNodeb;
vector<int> nElemb;
vector<vector<vector<int> > > bCond;
int nMark;


string NumberFinder(string line) {
  size_t i = 0;
  while (!isdigit(line[i]) && i < line.length())
  {
    i=i+1;
  }

  // remove the first chars, which aren't digits
  line = line.substr(i, line.length() - i );
  return line;
}

void MeshReader () {
  string line;
  ifstream myfile ("square_5x5.su2");
  if (myfile.is_open())
  {
    getline(myfile,line); //lit une premiere ligne
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }

    line = NumberFinder(line);
    NDIME = stoi(line);

    getline(myfile,line);
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }

    line = NumberFinder(line); // Find the number of elements
    NELEM = stoi(line);

    //getline(myfile, line); //lit une ligne
    //stringstream ss(line); // separe la ligne en mots
    //string word;  //declare le mot
    //ss >> word;  //assigne le premier mot
    //cout << word << '\n';

    vector<int> nNode;


    cell2node.reserve(NELEM);
    for (int i = 0; i<NELEM; i++)
    {
      // Find the number of nodes for one element using the su2 convention for the number of nodes
      // 3 -> Line (2), 5 -> Triangle (3), 9 -> Quadrilateral (4), 10 -> Tetrahedral (4), 12 -> Hexahedral (6), 13 -> Prism, 14 ->, Pyramid ->
      // Each element will be assigned a different number of nodes. Useful for unstructured meshes
      getline(myfile, line);
      stringstream ss_2(line);

      string word_2;
      ss_2 >> word_2;

      if (word_2.compare("3") == 0)
      {
        nNode.push_back(2);
        cell2nodeStart.push_back(2+cell2nodeStart[i]);
      }
      else if (word_2.compare("5") == 0)
      {
        nNode.push_back(3);
        cell2nodeStart.push_back(3+cell2nodeStart[i]);
      }
      else if (word_2.compare("9") == 0)
      {
        nNode.push_back(4);
        cell2nodeStart.push_back(4+cell2nodeStart[i]);
      }
      else
      {
        cout << "[ERROR] : Nombre de nodes pas reconnu" << '\n';
        break;
      }

      NFACE = cell2nodeStart[NELEM];

      // Store each node in the right line and in the right order in the cell2node vector
      ss_2 >> word_2;


      for (int j = 0; j<nNode[i]; j++)
      {
        cell2node.push_back(stoi(word_2));
        ss_2 >> word_2;

      }


    }


    getline(myfile,line); //lit une premiere ligne
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }
    line = NumberFinder(line); // Find the number of elements
    NPOIN= stoi(line);





    coord.reserve(NPOIN);
    for (int i = 0; i<NPOIN; i++)
    {
      // Find the number of nodes for one element using the su2 convention for the number of nodes
      // 3 -> Line (2), 5 -> Triangle (3), 9 -> Quadrilateral (4), 10 -> Tetrahedral (4), 12 -> Hexahedral (6), 13 -> Prism, 14 ->, Pyramid ->
      // Each element will be assigned a different number of nodes. Useful for unstructured meshes
      getline(myfile, line);
      stringstream ss_2(line);



      for (int j = 0; j<NDIME; j++)
      {
        string word_2;
        ss_2 >> word_2;
        double value = stod(word_2);
        coord[i].push_back(value);

      }
    }

    getline(myfile,line); //lit une premiere ligne
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }

    stringstream ss(line);

    // Store one word at a time
    string word;
    ss >> word;

    if (word.compare("NMARK=") == 0)
    {
      ss >> word;
      nMark = stoi(word);
      bCond.resize(nMark);

      // Create a loop to extract all the info on the boundary conditions
      for (int i = 0; i<nMark; i++)
      {
        getline(myfile, line);
        stringstream ss_2(line);

        string word_2;
        ss_2 >> word_2;

        // Make sure that the next line is the marker_tag
        if (word_2.compare("MARKER_TAG=") == 0)
        {
          getline(myfile, line);
          stringstream ss_3(line);
          string word_3;
          ss_3 >> word_3;

          // Find the number of elements per boundary condition
          if (word_3.compare("MARKER_ELEMS=") == 0)
          {
            ss_3 >> word_3;
            nElemb.push_back(stoi(word_3));
          }
          else
          {
            cout << "[ERROR] : Nombre d'éléments pas reconnu" << endl;
            break;
          }

          bCond[i].resize(nElemb[i]);

          // Stock the boundary point numbers
          for (int j = 0; j<nElemb[i]; j++)
          {
            getline(myfile, line);
            stringstream ss_4(line);
            string word_4;

            ss_4 >> word_4;

            // Find the number of nodes per boundary element
            if (word_4.compare("3") == 0)
            {
              nNodeb.push_back(2);
            }
            else if (word_4.compare("5") == 0)
            {
              nNodeb.push_back(3);
            }
            else if (word_4.compare("9") == 0)
            {

              nNodeb.push_back(4);
            }
            else
            {
              cout << "[ERROR] : Nombre de nodes pas reconnu" << endl;
              break;
            }

            for (int k = 0; k<nNodeb[j]; k++)
            {
              ss_4 >> word_4;
              bCond[i][j].push_back(stoi(word_4));
            }
          }
        }
        // Show error if "MARKER_TAG=" is not the next input
        else
        {
          cout << "[ERROR]: Could not read file correctly. Missing 'MARKER_TAG='" << endl;
          break;
        }
        NFACE += nElemb[i];
      }
    }

    NFACE = NFACE/2;

  myfile.close();
  }


  else cout << "Unable to open file";

}



// int main(int argc, char const *argv[]) {
//   MeshReader();
//   return 0;
// }
