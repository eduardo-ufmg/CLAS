#include "computeSVs.hpp"

#include <vector>

#include "squaredDistance.hpp"
#include "isgabrielEdge.hpp"

const Vertices computeSVs(const Vertices& vertices)
{
  const size_t vertexqtty = vertices.size();

  Vertices supportVertices;

  for (size_t i = 0; i < vertexqtty; ++ i) {
    for (size_t j = i + 1; j < vertexqtty; ++ j) {
      
      const Vertex& vi = vertices[i];
      const Vertex& vj = vertices[j];

      if (vi.cluster == vj.cluster) {
        continue;
      }

      bool isGE = isGabrielEdge(vertices, vi, vj, vertexqtty);

      if (isGE) {

      }

    }
  }
}
