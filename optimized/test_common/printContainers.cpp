#include "printContainers.hpp"

using namespace std;

void printVertices(const Vertices& vertices, size_t limitPerSegment)
{
  size_t counter = 0;

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

  limitPerSegment = min(limitPerSegment, vertices.size());

  printFromTo(0, limitPerSegment, vertices);

  size_t half = vertices.size() / 2;

  printFromTo(half, half + limitPerSegment, vertices);

  printFromTo(vertices.size() - limitPerSegment, vertices.size(), vertices);
}

void printVertexQuantity(const Vertices& vertices)
{
  cout << "Vertices: " << vertices.size() << endl;
}

void printClusterStats(const Vertices& vertices)
{
  Clusters clusters;

  for (const auto& vertex : vertices) {
    clusters.emplace(vertex.cluster->id, vertex.cluster);
  }

  for (const auto& [_, cluster] : clusters) { (void)_;
    cout << "Cluster " << cluster->id << '\n';
    cout << "  sumq: " << cluster->sumq << '\n';
    cout << "  magq: " << cluster->magq << '\n';
    cout << "  online_avgq: " << cluster->online_avgq << '\n';
    cout << "  online_stdq: " << cluster->online_stdq << '\n';
    cout << "  treshold: " << cluster->treshold << '\n';
  }
}
