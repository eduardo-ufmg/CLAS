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

VertexVector collectAllVerticesIntoVector(ClusterMap clusters);

void computeGabrielGraph(ClusterMap clusters)
{
  // Collect all vertices from all clusters.
  VertexVector allVertices = collectAllVerticesIntoVector(clusters);

  size_t verticesQty = allVertices.size();

  for (size_t i = 0; i < verticesQty; ++i) {
    VertexID viid = allVertices[i].first;
    shared_ptr<Vertex> vi = allVertices[i].second;

    for (size_t j = i + 1; j < verticesQty; ++j) {
      VertexID vjid = allVertices[j].first;
      shared_ptr<Vertex> vj = allVertices[j].second;

      if (viid == vjid) {
        continue;
      }

      vector<double> midPoint(vi->features.size());

      transform(vi->features.begin(), vi->features.end(),
        vj->features.begin(), midPoint.begin(),
        [](double a, double b) {
          return (a + b) / 2.0;
        });

      double sqRadius = squaredDistance(vi->features, midPoint);

      bool isEdge = true;

      for (auto [vkid, vk] : allVertices) {
        if (vkid == viid || vkid == vjid) {
          continue;
        }

        double sqDistance = squaredDistance(vk->features, midPoint);

        if (sqDistance < sqRadius) {
          isEdge = false;
          break;
        }

      }

      if (isEdge) {
        bool isSE = vi->cluster != vj->cluster;

        vi->adjacents.push_back({vjid, isSE});
        vj->adjacents.push_back({viid, isSE});
      }

    }
  }

}

VertexVector collectAllVerticesIntoVector(ClusterMap clusters)
{
  VertexVector allVertices;

  for (auto [_, cluster] : clusters) { (void)_;
    for (auto [vertexid, vertex] : cluster.vertices) {
      allVertices.push_back({vertexid, vertex});
    }
  }

  return allVertices;
}