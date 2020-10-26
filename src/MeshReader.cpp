// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

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
  ifstream myfile ("example.txt");
  if (myfile.is_open())
  {
    getline(myfile,line); //lit une premiere ligne
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }
    cout << line << '\n';
    line = NumberFinder(line);
    int NDIME = stoi(line);
    cout << NDIME << '\n';

    getline(myfile,line);
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }
    cout << line  << '\n';
    line = NumberFinder(line); // Find the number of elements
    int NELEM = stoi(line);
    cout << NELEM << '\n';

    //getline(myfile, line); //lit une ligne
    //stringstream ss(line); // separe la ligne en mots
    //string word;  //declare le mot
    //ss >> word;  //assigne le premier mot
    //cout << word << '\n';

    vector<int> nNode;
    vector<vector<int>> iNpoel;

    iNpoel.reserve(NELEM);
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
      }
      else if (word_2.compare("5") == 0)
      {
        nNode.push_back(3);
      }
      else if (word_2.compare("9") == 0)
      {
        nNode.push_back(4);

      }
      else
      {
        cout << "[ERROR] : Nombre de nodes pas reconnu" << '\n';
        break;
      }

      // Store each node in the right line and in the right order in the iNpoel vector
      ss_2 >> word_2;

      iNpoel.push_back({});

      for (int j = 0; j<nNode[i]; j++)
      {
        iNpoel[i].push_back(stoi(word_2));
        ss_2 >> word_2;

      }
    }

    getline(myfile,line); //lit une premiere ligne
    while (line[0] == '%') //skippe les lignes en commentaires
    {
      getline(myfile,line);
    }
    line = NumberFinder(line); // Find the number of elements
    int NPOIN= stoi(line);
    cout << NPOIN << '\n';


    vector<vector<double>> coord;

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
        coord[i].push_back({value});
        cout << coord[i][j] << "  ";
      }
      cout << '\n';
    }







    myfile.close();
  }

  else cout << "Unable to open file";

}



int main(int argc, char const *argv[]) {
  MeshReader();
  return 0;
}
