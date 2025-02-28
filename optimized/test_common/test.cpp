#include <iostream>

#include "types.hpp"
#include "printContainers.hpp"
#include "readFiles.hpp"
#include "gabrielGraph.hpp"
#include "filter.hpp"
#include "computeSVs.hpp"

const float DEFAULT_TOLERANCE = 0.5f;

using namespace std;

int main(int argc, char **argv)
{

  float tolerance = DEFAULT_TOLERANCE;

  if (argc > 1) {
    tolerance = stof(argv[1]);
  }

  Vertices vertices = readDataset("../../data/synthetic/synthetic");

  #if DEBUG
  cout << "DEBUG: READ_DATASET OK" << endl;
  #endif

  const size_t limitPerSegment = 2;

  computeGabrielGraph(vertices);

  cout << "After Gabriel Graph:" << endl;
  printVertices(vertices, limitPerSegment);

  filter(vertices, tolerance);

  cout << "After Filter:" << endl;
  printVertexQuantity(vertices);
  printClusterStats(vertices);

  const Vertices supportVertices = computeSVs(vertices);

  cout << "Support Vertices:" << endl;
  printVertices(supportVertices);

  return 0;
}