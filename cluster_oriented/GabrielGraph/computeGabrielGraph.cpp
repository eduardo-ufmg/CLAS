#include "computeGabrielGraph.hpp"

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>

#include "graphTypes.hpp"
#include "squaredDistance.hpp"

using namespace std;
using VertexVector = vector< pair<const VertexID, Vertex> >;

VertexVector collectAllVerticesIntoVector(const ClusterMap& clusters);
void writeAdjacentsToVerticesInClusters(const VertexVector& allVertices);

void computeGabrielGraph(ClusterMap& clusters)
{
  // Collect all vertices from all clusters.
  VertexVector allVertices = collectAllVerticesIntoVector(clusters);

  const size_t verticesQty = allVertices.size();

  for (size_t i = 0; i < verticesQty; ++i) {
    const VertexID viid = allVertices[i].first;
    Vertex * const vi = &allVertices[i].second;

    for (size_t j = i + 1; j < verticesQty; ++j) {
      const VertexID vjid = allVertices[j].first;
      Vertex * const vj = &allVertices[j].second;

      if (viid == vjid) {
        continue;
      }

      Coordinates midPoint(vi->features.size());

      transform(vi->features.begin(), vi->features.end(),
        vj->features.begin(), midPoint.begin(),
        [](const double a, const double b) {
          return (a + b) / 2.0;
        });

      const double sqRadius = squaredDistance(vi->features, midPoint);

      bool isEdge = true;

      for (const auto& [vkid, vk] : allVertices) {
        if (vkid == viid || vkid == vjid) {
          continue;
        }

        double sqDistance = squaredDistance(vk.features, midPoint);

        if (sqDistance < sqRadius) {
          isEdge = false;
          break;
        }

      }

      if (isEdge) {
        bool isSE = vi->cluster != vj->cluster;

        vi->adjacents.emplace_back(vjid, isSE);
        vj->adjacents.emplace_back(viid, isSE);
      }

    }
  }

  writeAdjacentsToVerticesInClusters(allVertices);

}

VertexVector collectAllVerticesIntoVector(const ClusterMap& clusters)
{
  VertexVector allVertices;

  for (const auto& [_, cluster] : clusters) { (void)_;
    for (const auto& [vertexid, vertex] : cluster.vertices) {
      allVertices.emplace_back(vertexid, vertex);
    }
  }

  return allVertices;
}

void writeAdjacentsToVerticesInClusters(const VertexVector& allVertices)
{
  for (const auto& [vertexid, vertex] : allVertices) {
    auto& targetAdjacents = vertex.cluster->vertices[vertexid].adjacents;
    targetAdjacents.reserve(vertex.adjacents.size());
    copy(vertex.adjacents.begin(), vertex.adjacents.end(), back_inserter(targetAdjacents));
  }
}
