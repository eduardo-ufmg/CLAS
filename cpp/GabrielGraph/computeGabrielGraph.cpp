#include "computeGabrielGraph.hpp"

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>

#include "graphTypes.hpp"
#include "squaredDistance.hpp"

using namespace std;
using VertexVector = vector< pair<VertexID, shared_ptr<Vertex> > >;

const VertexVector collectAllVerticesIntoVector(const ClusterMap& clusters);

void computeGabrielGraph(ClusterMap& clusters)
{
  // Collect all vertices from all clusters.
  const VertexVector allVertices = collectAllVerticesIntoVector(clusters);

  const size_t verticesQty = allVertices.size();

  for (size_t i = 0; i < verticesQty; ++i) {
    const VertexID viid = allVertices[i].first;
    const shared_ptr<Vertex>& vi = allVertices[i].second;

    for (size_t j = i + 1; j < verticesQty; ++j) {
      const VertexID vjid = allVertices[j].first;
      const shared_ptr<Vertex>& vj = allVertices[j].second;

      if (viid == vjid) {
        continue;
      }

      vector<double> midPoint(vi->features.size());

      transform(vi->features.begin(), vi->features.end(),
        vj->features.begin(), midPoint.begin(),
        [](double a, double b) {
          return (a + b) / 2.0;
        });

      const double sqRadius = squaredDistance(vi->features, midPoint);

      bool isEdge = true;

      for (const auto& [vkid, vk] : allVertices) {
        if (vkid == viid || vkid == vjid) {
          continue;
        }

        const double sqDistance = squaredDistance(vk->features, midPoint);

        if (sqDistance < sqRadius) {
          isEdge = false;
          break;
        }

      }

      if (isEdge) {
        const bool isSE = vi->cluster != vj->cluster;

        vi->adjacents.push_back({vjid, isSE});
        vj->adjacents.push_back({viid, isSE});
      }

    }
  }

}

const VertexVector collectAllVerticesIntoVector(const ClusterMap& clusters)
{
  VertexVector allVertices;

  for (const auto& [_, cluster] : clusters) { (void)_;
    for (const auto& [vertexid, vertex] : cluster.vertices) {
      allVertices.push_back({vertexid, vertex});
    }
  }

  return allVertices;
}