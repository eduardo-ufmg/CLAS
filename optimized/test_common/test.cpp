#include <iostream>

#include "types.hpp"
#include "printContainers.hpp"
#include "readFiles.hpp"
#include "gabrielGraph.hpp"


using namespace std;

int main()
{
  Vertices vertices = readDataset("../../data/synthetic/synthetic");

  #if DEBUG
  cout << "DEBUG: READ_DATASET OK" << endl;
  #endif

  printVertices(vertices, 5);

  computeGabrielGraph(vertices);

  printVertices(vertices, 5);

  return 0;
}