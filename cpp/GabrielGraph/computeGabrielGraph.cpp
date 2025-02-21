#include "computeGabrielGraph.hpp"

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>

#include "graphTypes.hpp"

using namespace std;
using VertexVector = vector< pair<VertexID, shared_ptr<Vertex> > >;

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b);
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

      for (auto& [vkid, vk] : allVertices) {
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

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b)
{
  double sum = 0.0;
  size_t n = a.size();

  if (n != b.size()) {
   return numeric_limits<double>::infinity();
  }

  for (size_t i = 0; i < n; ++i) {
    double diff = a[i] - b[i];
    sum += diff * diff;
  }

  return sum;
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