#include "computeSVs.hpp"

#include <vector>
#include <algorithm>

#include "squaredDistance.hpp"
#include "isgabrielEdge.hpp"

using namespace std;

bool insert_unique_sorted(Vertices& Vertices, const Vertex& vertex);

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
        insert_unique_sorted(supportVertices, vi);
        insert_unique_sorted(supportVertices, vj);
      }

    }
  }

  return supportVertices;
}

bool insert_unique_sorted(Vertices& Vertices, const Vertex& vertex)
{
    auto it = lower_bound(Vertices.begin(), Vertices.end(),
                          vertex,
                          [](const Vertex& a, const Vertex& b) {
                              return a.id < b.id;
                          });

    if (it != Vertices.end() // a Vertex that is not less was found
        && it->id == vertex.id) { // the Vertex is the same
      return false;
    }

    Vertices.insert(it, vertex);
    return true;
}
