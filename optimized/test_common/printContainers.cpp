#include "printContainers.hpp"

using namespace std;

void printVerticesFromTo(const Vertices& vertices, size_t from, size_t to);

void printVertices(const Vertices& vertices)
{
  printVerticesFromTo(vertices, 0, vertices.size());
}

void printVertices(const Vertices& vertices, size_t limitPerSegment)
{
  size_t counter = 0;

  limitPerSegment = min(limitPerSegment, vertices.size());

  printVerticesFromTo(vertices, 0, limitPerSegment);

  size_t half = vertices.size() / 2;

  printVerticesFromTo(vertices, half, half + limitPerSegment);

  printVerticesFromTo(vertices, vertices.size() - limitPerSegment, vertices.size());
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

void printVerticesFromTo(const Vertices& vertices, size_t from, size_t to)
{
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
}

