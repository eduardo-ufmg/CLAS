#include "printContainers.hpp"

using namespace std;

void printVertices(const Vertices& vertices, size_t limitPerSegment)
{
  size_t counter = 0;

  limitPerSegment = min(limitPerSegment, vertices.size());

  auto printFromTo = [](size_t from, size_t to, const Vertices& vertices) {
    for (size_t i = from; i < to; ++i) {
      const auto& vertex = vertices[i];
      cout << vertex.id << " ";

      if (vertex.cluster) {
        cout << "(" << vertex.cluster->id << ") ";
      }

      for (const auto& c : vertex.coordinates) {
        cout << c << " ";
      }

      if (!vertex.adjacencyList.empty()) {
        cout << "|";

        for (const auto& adj : vertex.adjacencyList) {
          cout << " (" << adj.first->id << " - " << adj.second << ")";
        }

      }

      cout << endl;
    }
  };

  printFromTo(0, limitPerSegment, vertices);

  size_t half = vertices.size() / 2;

  printFromTo(half, half + limitPerSegment, vertices);

  printFromTo(vertices.size() - limitPerSegment, vertices.size(), vertices);
}
