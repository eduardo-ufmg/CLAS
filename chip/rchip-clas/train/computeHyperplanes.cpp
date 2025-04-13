#include "computeHyperplanes.hpp"

#include <algorithm>

#include "isgabrielEdge.hpp"

using namespace std;

bool emplace_unique(Hyperplanes& hyperplanes, const HyperplaneID id, const Vertex& lowerVertex, const Vertex& higherVertex);

const Hyperplanes computeHyperplanes(const Vertices& vertices)
{
  const size_t vertexqtty = vertices.size();

  Hyperplanes hyperplanes;
  HyperplaneID hyperplaneid = 0;

  for (size_t i = 0; i < vertexqtty; ++ i) {
    for (size_t j = i + 1; j < vertexqtty; ++ j) {
      
      const Vertex& vi = vertices[i];
      const Vertex& vj = vertices[j];

      if (vi.cluster == vj.cluster) {
        continue;
      }

      bool isGE = isGabrielEdge(vertices, vi, vj, vertexqtty);

      if (isGE) {
        const Vertex& lowerVertex = vi.id < vj.id ? vi : vj;
        const Vertex& higherVertex = vi.id > vj.id ? vi : vj;        
        emplace_unique(hyperplanes, hyperplaneid ++, lowerVertex, higherVertex);
      }

    }
  }

  return hyperplanes;
}

bool emplace_unique(Hyperplanes& hyperplanes, const HyperplaneID id, const Vertex& lowerVertex, const Vertex& higherVertex)
{
  auto it = find_if(hyperplanes.begin(), hyperplanes.end(), [&lowerVertex, &higherVertex](const auto& hyperplane) {
    return hyperplane.edge.first->id == lowerVertex.id && hyperplane.edge.second->id == higherVertex.id;
  });

  if (it != hyperplanes.end()) {
    return false;
  }

  const Edge edge = make_pair(&lowerVertex, &higherVertex);
  hyperplanes.emplace_back(id, edge);

  return true;
}
