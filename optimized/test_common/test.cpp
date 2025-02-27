#include <iostream>

#include "types.hpp"
#include "readFiles.hpp"

using namespace std;

int main()
{
  Vertices vertices = readDataset("../../data/synthetic");

  for (const Vertex& vertex : vertices) {
    cout << "Vertex " << vertex.id << " in cluster " << vertex.cluster->id << " has coordinates: ";

    for (const float& coord : vertex.coordinates) {
      cout << coord << " ";
    }

    cout << endl;
  }

  return 0;
}